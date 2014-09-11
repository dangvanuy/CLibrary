/*
   (c) Copyright 2010 Sigma Designs

   Proprietary and confidential.
   */

//#include <config.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <semaphore.h>
#include <termios.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/keyboard.h>

#include <stdio.h>
#include <string.h>
#include <signal.h>

#ifndef POWER_CONTROL
#define POWER_CONTROL 1 //for board have power chip
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>

#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_util.h>

#include <direct/clock.h>
#include <direct/messages.h>
#include <direct/list.h>

#include <sawman.h>

#include <tinyxml.h>

extern "C" {
#include <divine.h>
}

#include <common.h>
#include "FullScreenLayout.h"

#include "app.h"
#include "media.h"

#ifdef POWER_CONTROL
	#define ALLOW_OS_CODE 1
	#include "rmdef/rmdef.h"
	#include "llad/include/gbus.h"
	#include "emhwlib_hal/include/emhwlib_registers.h"
	#include <sys/time.h>
	#include <time.h>

	RM_EXTERN_C_BLOCKSTART
	#include "emhwlib_hal/i2c/include/i2c_hal.h"
	#include "tuner/helper/helper.h"
	#include "tuner/emi2c/emi2c.h"
	RM_EXTERN_C_BLOCKEND
	
	RMuint32 size = 1;
	struct llad* pLLAD;
	RMvoid *pGBus;
	struct i2c  i2cConfig;
	I2CAL_CONTEXT*  pI2Cal_Context;
	RMstatus r;
	RMuint32 countPower = 0;
	unsigned char ucTunerByte[300];
	RMuint8 ucReadByte[10];
	struct timeval start, end, current;
	long long mtime, seconds, useconds;
#endif

unsigned int isOldTestGame;

#define UNUSED( x ) do{ x = x; }while(0)

#define CHECK(x)                                  \
   do {                                         \
      DFBResult ret = (DFBResult)(x);         \
      if (ret != DFB_OK) {                    \
         DirectFBError(#x,ret);             \
         goto out;                          \
      }                                       \
   } while (0)

#define BREAK_ON_NULL( X ) if( NULL == (X) ){ fprintf(stderr, "BREAK_ON_NULL: line %d", __LINE__); break;};

//#define DRAWING_POS_X 100
//#define DRAWING_POS_Y 100

typedef struct 
{
   IDirectFB*        pDfb;
   ISaWMan*          pSaw;
   ISaWManManager*   pManager;
   FullScreenLayout* pLayout;
   int               fifo;
   char              cmd_str[SYSTEM_MAX_CMD_LEN];
   command_t         cmd;
} context_t;


static context_t      gCtx;
static volatile bool  gbExit = false;      

static void         launch_next_app( bool );
static int          command_parse( command_t*, char*);
static DirectResult start_request( void*, const char*, pid_t* );
static DirectResult stop_request( void*, pid_t, FusionID );
static DirectResult process_added( void*, SaWManProcess* );
static DirectResult process_removed( void*, SaWManProcess* );
static DirectResult input_filter( void*, DFBInputEvent* );
static DirectResult window_preconfig( void*, SaWManWindowConfig* );
static DirectResult window_added( void*, SaWManWindowInfo* );
static DirectResult window_removed( void*, SaWManWindowInfo* );
static DirectResult window_reconfig( void*, SaWManWindowReconfig* );
static DirectResult window_restack( void*, SaWManWindowHandle, SaWManWindowHandle,
                                    SaWManWindowRelation );
static DirectResult switch_focus( void* , SaWManWindowHandle );
static DirectResult application_id_changed( void* , SaWManWindowInfo* );




/**************************************************************************************/


#if 0
static const DirectFBKeySymbolNames(dfb_keynames);

static DFBInputDeviceKeySymbol input_symbol_from_string(const char *str);

static DFBInputDeviceKeySymbol input_symbol_from_string(const char *str)
{
   unsigned i;

   for (i=0; i<sizeof(dfb_keynames)/sizeof(dfb_keynames[0]); ++i)
      if (strcasecmp(dfb_keynames[i].name, str) == 0)
         return dfb_keynames[i].symbol;

   return DIKS_NULL;
}



/**
 * Catch child exits that are unexptected.  Forward the pid that exited to sawman to cleanup.
 *
 * \param[in] sig
 *            The signal that was received, should be SIGCHLD, but is ignored.
 */
static void         sigchld_handler( int );
static void sigchld_handler( int sig )
{
   app_t* pApp = NULL;
   pid_t  pid = 0;
   int    status = 0;

   sig = sig;
   
   while(( pid = waitpid( -1, &status, WNOHANG )) > 0 )
   {
      if( WIFSIGNALED( status ))
      {
         D_INFO( "Application w/ PID %d exited by Signal %d\n",
                  pid, WTERMSIG( status ));
      }
      else if( WIFEXITED( status ))
      {
         D_INFO( "Application w/ PID %d exited normally, returning %d\n",
                  pid, WEXITSTATUS( status ));
      }

      pApp = app_find( pid );
      if( NULL != pApp )
      {
         /* If we can obtain the mutex, then go ahead and process the signal.  If we cannot then
            someone else is handling it, so don't do anything */
         if( 0 == app_try_lock( pApp ))
         {
            //pApp->bWaited = true;	//Thinh close
            /* Detect a crashed UI task.  Only treat as a crash if it terminated because
            ** of a signal, or if it terminated by calling exit() with a non-zero value. */ 
            if(( NULL != pApp->pName ) && ( !strcmp( pApp->pName, "ui" )) &&
               (( true == WIFSIGNALED( status )) ||
                ( /*true == WIFEXITED( status ) &&*/ ( 0 != WEXITSTATUS( status ))) ))
            {
               D_INFO( "Launcher UI Crashed, exiting\n" );
               /* Crashed running the launcher UI, so just exit */
               //gbExit = true;	//Thinh close
            }
            app_unlock( pApp );

            gCtx.pSaw->Stop( gCtx.pSaw, pid );
         }
      }
      else
      {
         /* No app reference, so stop the pid */
         gCtx.pSaw->Stop( gCtx.pSaw, pid );
      }
   }
}
#endif
/**
 * A process has quit or died.  We now need to determine if there is an appl to launch.  If not,
 * then we relaunch the launcher UI.
 *
 * \param[in] bLaunchUI
 *            Force the next app to be the launcher UI.
 */
static void launch_next_app( bool bLaunchUI )
{
	//printf(">>>>>launch_next_app<<<<<<<<<\n");
/*
	usleep(2000);
	init_osd_layer(false);
	init_gui();

    show_menu_blue();
	show_window();
*/
   pid_t pid = 0;
   int   len = 0;
   
   do
   {
      if( gbExit )
      {
         // We are exiting so do nothing
         bLaunchUI = false;
         break;
      }
		
      if( true == bLaunchUI ) {
         break;
      }
      
      // Try to read a command from the fifo
      len = read( gCtx.fifo, gCtx.cmd_str, SYSTEM_MAX_CMD_LEN-1 );
      //D_INFO("SaWMan/Applauncher: Command len: %d\n", len);
      if( len <= 0 )
      {
         // No command available so relaunch UI
         D_INFO("SaWMan/Applauncher: No command found. Restarting the UI\n");
         bLaunchUI = true;
         break;
      }
	  

      gCtx.cmd_str[len] = '\0';

      //D_INFO("SaWMan/Applauncher: Found application command: \"%s\"\n", gCtx.cmd_str);

      // Parse the command
      if( 0 != command_parse( &gCtx.cmd, gCtx.cmd_str ))
      {
         LOG_ERROR("Failed to parse command....\n");
         break;
      }
      // Process the command
      gCtx.pSaw->Start( gCtx.pSaw, "app", &pid );

      bLaunchUI = false;
   } while(0);

   // Do we need to launch the UI again?
   if( true == bLaunchUI )
   {
      gCtx.pSaw->Start( gCtx.pSaw, "ui", &pid );
   }

   return;
}


/**
 * Parse out a command
 *
 * \return Returns 0 on success, less then zero on an error
 *
 */
static int command_parse( command_t* pCmd, char* str )
{
   int   rc = -1;
   char* tmp = NULL;
   char* pSave = NULL;
   int   ii = 0;

   do
   {
      // Clear out the command struct
      memset(pCmd, 0, sizeof(command_t));

      pCmd->cmd = COMMAND_INVALID;

      tmp = strtok_r(str, ",", &pSave); 

      // Check for the "CMD" prefix
      if( 0 != strcmp(tmp, "CMD") )
      {
        LOG_ERROR("Invalid Command: Command does not contain proper prefix.\n");
        break;
      }

      // Pull out the command ID
      tmp = strtok_r(NULL, ",", &pSave);

      if( '0' == tmp[0] )
      {
         // TODO: Handle Command Quit
         pCmd->cmd = COMMAND_QUIT;
      }
      else if( '1' == tmp[0] )
      {
         pCmd->cmd = COMMAND_APP;

         // Get the app name
         BREAK_ON_NULL(pCmd->name = strtok_r(NULL, ",", &pSave));

         // Get the app command 
         BREAK_ON_NULL(pCmd->app = strtok_r(NULL, ",", &pSave));

         // Get the command path
         BREAK_ON_NULL(pCmd->path = strtok_r(NULL, ",", &pSave));

         // Get the number of args 
         BREAK_ON_NULL(tmp = strtok_r(NULL, ",", &pSave));

         pCmd->num_args = atoi(tmp);

         // Parse the args if there are any
         ii = 0;

         // Put the app name as the first arg
         pCmd->args[ii++] = pCmd->app;

         while( (ii < (pCmd->num_args+1)) && (NULL != (tmp = strtok_r(NULL, ",", &pSave))) )
         {
            pCmd->args[ii++] = tmp;

            if( ii >= (COMMAND_MAX_ARGS + 1) )
            {
               D_INFO("WARNING: Too many command arguments! Max allowed is %d.\n", COMMAND_MAX_ARGS);
               break;
            }
         }

         // Make the last arg NULL. This is why we add 1 to the COMMAND_MAX_ARGS. :)
         pCmd->args[ii] = NULL;

         if( ii != (pCmd->num_args + 1) )
         {
            LOG_ERROR("Command Error: Failed to read correct number of arguments. Expected %d but found %d.\n", pCmd->num_args, ii - 1);
            break;
         }

         // Now parse any env vars
         BREAK_ON_NULL(tmp = strtok_r(NULL, ",", &pSave));

         pCmd->num_envs = atoi(tmp);

         // Parse the envs if there are any
         ii = 0;

         while( ii < pCmd->num_envs )
         {
            // Get bReplace
            BREAK_ON_NULL(tmp = strtok_r(NULL, ",", &pSave));

            pCmd->envs[ii].bReplace = atoi(tmp);

            // Get the env var name
            BREAK_ON_NULL(pCmd->envs[ii].name = strtok_r(NULL, ",", &pSave));

            // Get the env var value
            BREAK_ON_NULL(pCmd->envs[ii].value = strtok_r(NULL, ",", &pSave));

            ii++; 

            if( ii >= COMMAND_MAX_ENVS )
            {
               D_INFO("WARNING: Too many command environmanet vars! Max allowed is %d.\n", COMMAND_MAX_ENVS);
               break;
            }
         }

         if( ii != pCmd->num_envs )
         {
            LOG_ERROR("Command Error: Failed to read correct number of env vars. Expected %d but found %d.\n", 
                  pCmd->num_envs, ii);
            break;
         }

      }
      else
      {
         LOG_ERROR("Invalid Command: Unknown command type!\n");
         break;
      }

#if 0
      //printf("Command %d: %s, %s, %s, %d, %d\n",
            pCmd->cmd, pCmd->name, pCmd->app, pCmd->path, pCmd->num_args, pCmd->num_envs);  
      for( ii = 0; ii < pCmd->num_args; ii++ )
      {
         //printf("\targ %02d: %s\n", ii, pCmd->args[ii]);
      }
      for( ii = 0; ii < pCmd->num_envs; ii++ )
      {
         //printf("\tenv %02d: bReplace:%d, %s=%s\n",
               ii, pCmd->envs[ii].bReplace, pCmd->envs[ii].name, pCmd->envs[ii].value);
      }
#endif
      rc = 0;

   } while( 0 );

   return( rc );
}

/**
 * start_request
 *
 */
static DirectResult start_request( void* context, const char* name, pid_t* ret_pid )
{
	//uninit_gui();
	//uninit_osd_layer();

   const char* path = NULL;
   pid_t       pid = 0;
   int         rc = 0;

   // Make a copy of name because it can get trashed after the fork
   int len = strlen(name);
   char* tmpName = (char*)malloc(len + 1);
   strcpy(tmpName, name);

   UNUSED( context );

   D_INFO("SaWMan/AppLauncher: Start request for '%s'!\n", name);

   D_ASSERT(name != NULL);
   D_ASSERT(ret_pid != NULL);

   //app->started = true;
   //app->start_time = direct_clock_get_millis();

   pid = fork(); //vfork();

   switch (pid)
   {
      case -1:
         perror("vfork");
         return DR_FAILURE;

      case 0: // child process
         setsid();

         do
         {
            if( 0 != strcmp(tmpName, "app") )
            {
               break;
            }
            D_INFO("SaWMan/AppLauncher: Launching \"%s\"\n", gCtx.cmd.name);

            rc = chdir(gCtx.cmd.path);

            if( 0 != rc )
            {
               LOG_ERROR("Failed to change to directory: \"%s\"\n", gCtx.cmd.path);
               perror("chdir");
               break;
            }

            // Set all env vars
            for( int ii = 0; ii < gCtx.cmd.num_envs; ii++ )
            {
               command_env_t* pEnv = &gCtx.cmd.envs[ii];

               if( pEnv->bReplace == 0 ) // replace
               {
                  D_INFO("Replacing env: %s = %s\n", pEnv->name, pEnv->value);
                  setenv(pEnv->name, pEnv->value, 1);
               }
               else
               {
                  char* old_value = getenv(pEnv->name);

                  int added_env_len = strlen(pEnv->value);
                  int old_env_len = 0;
                  int new_len;
                  
                  if ( old_value )
                  {
                      old_env_len = strlen(old_value);
                      // Extra +1 to add a ':'
                      new_len = added_env_len + old_env_len + 1;
                  }
                  else
                      new_len = added_env_len;

                  char* tmp = (char*)malloc(new_len + 1 );

                  if ( NULL == old_value )
                      strcpy( tmp, pEnv->value );
                  else if ( pEnv->bReplace == 1 )
                  {
                      // prepend
                      strcpy( tmp, pEnv->value );
                      tmp[added_env_len] = ':';
                      strcpy( &tmp[added_env_len + 1], old_value );
                  }
                  else
                  {
                      // append
                      strcpy( tmp, old_value );
                      tmp[old_env_len] = ':';
                      strcpy( &tmp[old_env_len + 1], pEnv->value );                      
                  }

                  D_INFO("%spending env: %s = %s\n", pEnv->bReplace == 1? "Pre" : "Ap", pEnv->name, tmp);
                      
                  setenv(pEnv->name, tmp, 1);

                  free( tmp );
               }
            }

            rc = execvp(gCtx.cmd.app, gCtx.cmd.args);

            /* If we get here we have a problem. We'll fall through and start the UI */
            LOG_ERROR("Failed to launch \'%s\'\n", tmpName);
            perror("execvp");

         } while(0);

         // TODO: Load this from env vars or config?
         D_INFO("SaWMan/AppLauncher: Switching to UI\n");

         chdir("/tango/app");

         path = getenv( "LAUNCHER_HTML_DIR" );
         if( NULL == path )
         {
			path="/tango/app";
         }
         
         chdir("/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full");
		 //printf(">>>>>>>>>>>>>>>>>>> call BrowserTest --- \n");
		 rc = execlp("./MainApp", "MainApp", NULL, NULL );
         LOG_ERROR("Failed to launch UI\n");
         perror("execvp");
         _exit(0);

      default:    // parent process

         app_add( pid, tmpName );
         
         D_INFO("SawMan/AppLauncher: Parent process...\n");

         break;
   }

   free( tmpName );

   *ret_pid = pid;

   return DR_OK;
}

#define MAX_PID_WAIT 5000   // 5 seconds

/**
 * stop_request
 *
 */
static DirectResult stop_request( void* context, pid_t pid, FusionID caller )
{
   long long t0 = 0;
   app_t*    pApp = NULL;
   bool      bDone = false;

   UNUSED( context );

   D_INFO("SaWMan/AppLauncher: Stop request from Fusion ID 0x%lx for pid %d!\n", caller, pid);

   pApp = app_find( pid );
   if( NULL == pApp )
   {
      D_INFO( "SawMan/AppLauncher: stop request on unknown pid\n" );
      return DR_OK; 
   }
   
   if( 0 == app_lock( pApp ))
   {
      if( true == pApp->bWaited )
      {
         bDone = true;
      }

      /* Wait a while; if the process doesn't die in MAX_PID_WAIT milliseconds, kill it */
      t0 = direct_clock_get_millis();
      while(( true != bDone ) && ( direct_clock_get_millis() - t0 <= MAX_PID_WAIT ))
      {
         //printf( "%s: waiting for process %d\n", __func__, pid );
         if( waitpid( pid, NULL, WNOHANG ))
         {
            bDone = true;
            break;
         }

         usleep(100000);
      }

      app_unlock( pApp );

      if( true != bDone )
      {
         /* Terminate the uncooperating app */
         D_ERROR("SaWMan/AppLauncher: Application hasn't exited; killing it.\n");
         kill( pid, 9 );
      }
      else
      {
         app_remove( pid );
         launch_next_app( true );
      }
   }
   
   return DR_OK;
}


/**
 * process_added
 *
 */
static DirectResult process_added( void* context, SaWManProcess* process )
{
   app_t* pApp = NULL;

   UNUSED( context );

   pApp = app_find( process->pid );
   if( NULL != pApp )
   {
      if( 0 == app_lock( pApp ))
      {
         pApp->pWmProc = process;
         app_unlock( pApp );
      }
   }
   
   D_INFO("SaWMan/AppLauncher: Process added (%d) [%lu]!\n", process->pid, process->fusion_id);

   return DR_OK;
}

/**
 * process_removed
 *
 */
static DirectResult process_removed( void* context, SaWManProcess* process )
{
//   context_t* pCtx = (context_t*)context;
   app_t*     pApp = NULL;
   bool       bCrashed = false;

   context = context;
   
   D_INFO("SaWMan/AppLauncher: Process removed (%d) [%lu]!\n", process->pid, process->fusion_id);

   pApp = app_find( process->pid );
   if( NULL == pApp )
   {
      D_INFO( "Unknown process is being removed\n" );
      return DR_OK;
   }
   
   if( 0 == app_lock( pApp ))
   {
      if( true == pApp->bWaited )
      {
         D_INFO("Previous application crashed\n" );
         bCrashed = true;
      }
   
      if( true != bCrashed )
      {
         //printf( "%s: waiting for process %d\n", __func__, process->pid );
         if( waitpid(process->pid, NULL, 0) < 0 )
         {
            perror("waitpid");
         }
      }
      D_INFO("SaWMan/AppLauncher: Launch again...\n");

      app_unlock( pApp );
   }

   app_remove( process->pid );

   //if( true != gbExit )
   {
      launch_next_app( bCrashed );
   }

   return DR_OK;
}

/**
 * input_filter
 *
 */
static DirectResult input_filter( void* context, DFBInputEvent* event )
{
	printf("input_filter --> event->type = (%x), press key = 0x%d\n\n", event->type, event->key_symbol);
   //ISaWManManager *manager;
   DirectResult    ret = DR_OK;
   static volatile bool exiting = false;

   UNUSED( context );

   //D_INFO("SaWMan/Applauncher: Input filter (%x) [exiting = %d]!\n", event->type, exiting);

   if (exiting)
      return DR_DESTROYED;

   //D_ASSERT(manager != NULL);

   //manager->Lock(manager);
   ////printf("\n--VAN LE: launchd.cpp --> input_filter() --> event->key_symbol = (0x%x)\n", event->key_symbol);
	////printf("\n--VAN LE: launchd.cpp --> input_filter() --> event->key_symbol = (0x%x)\n", event->key_symbol);

   switch (event->type)
   {
      case DIET_KEYPRESS:
     		//printf("\n--VAN LE: launchd.cpp --> input_filter() --> event->key_symbol = (0x%x)\n", event->key_symbol);
#ifdef POWER_CONTROL
    	  //I2Cal_Read_Subaddress(pI2Cal_Context, 0x69, 0x22, ucReadByte, 4);
			////printf("ucReadByte[0] = 0x%x\n", ucReadByte[0]);
			if (event->key_symbol != DIKS_POWER) {
				countPower = 0;
			}
			if (event->key_symbol == DIKS_POWER) {
				printf("press key power\n");
				gettimeofday(&end, NULL);
				seconds  = end.tv_sec  - start.tv_sec;
				useconds = end.tv_usec - start.tv_usec;
				mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
				gettimeofday(&start, NULL);
				if (mtime < 300) {
					//printf("countPower = %ld\n", countPower);
					countPower++;
				}
				else {
					countPower = 0;
				}
				if (countPower == 20) {
					countPower = 0;
					size = 1;
					memset(&i2cConfig, 0, sizeof(struct i2c) );
					i2cConfig.pGBus = (gbus*) pGBus;
					i2cConfig.PioClock = 0;
					i2cConfig.PioData = 1;
					i2cConfig.RegBase = REG_BASE_system_block;
					i2cConfig.DelayUs = 0x8000;	 // for emi2c, this will use default, for i2c_hal, set to 10

					pI2Cal_Context = (I2CAL_CONTEXT*)&i2cConfig;

					ucTunerByte[0] = 0x32;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x10, ucTunerByte, &size);

					ucTunerByte[0] = 0xfc;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x08, ucTunerByte, &size);
					ucTunerByte[0] = 0x80;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x11, ucTunerByte, &size);
					sleep(1);

					ucTunerByte[0] = 0xb0;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x20, ucTunerByte, &size);

					ucTunerByte[0] = 0x81;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x21, ucTunerByte, &size);

					ucTunerByte[0] = 0xa0;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x30, ucTunerByte, &size);

					ucTunerByte[0] = 0xb5;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x32, ucTunerByte, &size);
					ucTunerByte[0] = 0x4a;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x33, ucTunerByte, &size);
					ucTunerByte[0] = 0xcb;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x34, ucTunerByte, &size);
					ucTunerByte[0] = 0x04;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x35, ucTunerByte, &size);

					ucTunerByte[0] = 0xf5;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x38, ucTunerByte, &size);
					ucTunerByte[0] = 0x0a;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x39, ucTunerByte, &size);
					ucTunerByte[0] = 0x02;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x3a, ucTunerByte, &size);
					ucTunerByte[0] = 0x02;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x3b, ucTunerByte, &size);

					ucTunerByte[0] = 0x80;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x11, ucTunerByte, &size);

					ucTunerByte[0] = 0x81;
					I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x06, ucTunerByte, &size);

					I2Cal_Read_Subaddress(pI2Cal_Context, 0x69, 0x11, ucReadByte, &size);
					//printf("ucReadByte[0] = 0x%x\n", ucReadByte[0]);

					usleep(50000);
				}
			} else if (event->key_symbol == DIKS_ENTER){
				printf("app-launcer: press key DIKS_ENTER\n");
			} else if (event->key_symbol == DIKS_MENU){
				printf("app-launcer: press key DIKS_MENU\n");
			}
#endif	
         break;
      case DIET_KEYRELEASE:
         break;

      default:
         break;
   }

   //manager->Unlock(manger);
   return ret;
}


/**
 * window_preconfig
 *
 */
static DirectResult window_preconfig( void* context, SaWManWindowConfig* config )
{
   UNUSED(context);

   D_INFO("SaWMan/AppLauncher: Window preconfig (%d,%d-%dx%d)!\n", DFB_RECTANGLE_VALS(&config->bounds));

   return DR_OK;
}

/**
 * window_added
 *
 */
static DirectResult window_added( void* context, SaWManWindowInfo* info )
{
   UNUSED(  context );

   D_INFO("SaWMan/AppLauncher: Window added (%d,%d-%dx%d)!\n",
         DFB_RECTANGLE_VALS(&info->config.bounds));

   if( info->caps & DWCAPS_NODECORATION )
      return DR_NOIMPL;  /* to let sawman insert the window */

   /* Already showing window? (reattaching) */
   if( info->config.opacity )
   {
      /* Activate scaling. */
      info->config.options = (DFBWindowOptions)(info->config.options | DWOP_SCALE);

      gCtx.pLayout->AddWindow(info->handle);
   }

   return DR_OK;
}

/**
 * window_removed
 *
 */
static DirectResult window_removed( void* context, SaWManWindowInfo* info )
{
   UNUSED(context);

   D_INFO("SaWMan/AppLauncher: Window removed (%d,%d-%dx%d)!\n", DFB_RECTANGLE_VALS(&info->config.bounds));

   return DR_OK;
}

/**
 * window_reconfig
 *
 */
static DirectResult window_reconfig( void* context, SaWManWindowReconfig *reconfig )
{
   SaWManWindowConfig *current;
   SaWManWindowConfig *request;

   UNUSED(  context );

   D_INFO("SaWMan/AppLauncher: Window reconfig.\n");

   if (reconfig->caps & DWCAPS_NODECORATION)
      return DR_OK;

   current = &reconfig->current;
   request = &reconfig->request;

   if (reconfig->flags & SWMCF_OPACITY)
   {
      /* Show? */
      if (request->opacity && !current->opacity)
      {
         /* Activate scaling. */
         if (!(reconfig->flags & SWMCF_OPTIONS))
            request->options = DWOP_NONE;   /* if a flag is not set, corresponding field may not be initialised */

         reconfig->flags  = (SaWManWindowConfigFlags)(reconfig->flags | SWMCF_OPTIONS);
         request->options = (DFBWindowOptions)(request->options | DWOP_SCALE);

         gCtx.pLayout->AddWindow(reconfig->handle);
      }
      /* Hide? */
      else if (!request->opacity && current->opacity)
      {
         gCtx.pLayout->RemoveWindow(reconfig->handle);
      }
   }

   return DR_OK;
}

/**
 * window_restack
 *
 */
static DirectResult window_restack( void* context, SaWManWindowHandle handle, SaWManWindowHandle relative, SaWManWindowRelation relation )
{
   UNUSED(context);
   UNUSED(handle);
   UNUSED(relative);
   UNUSED(relation);

   D_INFO("SaWMan/AppLauncher: Window restack\n");

   return DR_OK;
}

/**
 * switch_focus
 *
 */
static DirectResult switch_focus( void* context, SaWManWindowHandle window )
{
   UNUSED(  context );

   D_INFO("SaWMan/AppLauncher: Switching focus to %lx\n", window);

   return DR_OK;
}

/**
 * application_id_changed
 *
 */
static DirectResult application_id_changed( void* context, SaWManWindowInfo *info)
{
   UNUSED(context);

   D_INFO("SaWMan/AppLauncher: Application ID switched to %ld\n", info->application_id);

   return DR_OK;
}

#if 0
static void usage(char *app_name)
{
   fprintf(stderr, "Usage: %s <config file>\n", app_name);
}
#endif

#if 1
    IDirectFBScreen *dfb_osd_screen = NULL;
    IDirectFBDisplayLayer *m_layer;
    IDirectFBWindow       *m_wnd;
    IDirectFBSurface      *m_surf;
    IDirectFBFont         *m_font_big;
	
	IDirectFBWindow       *m_wnd2;
	IDirectFBSurface      *m_surf2;

IDirectFBImageProvider *provider = NULL; 
DFBSurfaceDescription sdesc;
extern IDirectFBSurface   *slidesurface;

DFBResult load_image(char *filename, DFBRectangle DFBrect){
	//printf("\n ----> LoadImage (%s) \n", filename);
	DFBResult err = DFB_OK;
	DFBRectangle rect;
	if (provider == NULL) {
	    //printf("\n CreateImageProvider \n");
		err = gCtx.pDfb->CreateImageProvider(gCtx.pDfb, filename, &provider);
	}

	if (err != DFB_OK){		
		//printf("\n ERR_CREATE_IMAGE_PROVIDER \n");
		return err;
	} else {
		//printf("\n CreateImageProvider success\n");
	}

	provider->GetSurfaceDescription(provider, &sdesc);

	rect.x = rect.y = 0;
	m_surf->GetSize(m_surf, &rect.w, &rect.h);

	sdesc.flags  = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT /*| DSDESC_PIXELFORMAT*/);
	sdesc.caps = DSCAPS_VIDEOONLY;

	sdesc.width = rect.w;
	sdesc.height = rect.h;

	err = gCtx.pDfb->CreateSurface(gCtx.pDfb, &sdesc, &slidesurface);
	if (err == DFB_OK)
	{
		slidesurface->Clear(slidesurface, 0, 0, 0, 0);
		err = provider->RenderTo(provider, slidesurface, &DFBrect);
		if (err != DFB_OK) {			
			//printf("\n ERR_RENDER_TO_SURFACE \n");
			return err;
		}
	} else {
		provider->Release(provider);		
		//printf("\n provider->Release(provider) \n");
		return err;
	}


	provider->Release(provider);
	provider = NULL;
	return DFB_OK;
}

void BlitSurface(int x, int y, int w, int h);
void RunBlit(void);
void *run_text_thread(void* x);
int m_x = 450; 
int m_y = 280;

void BlitSurface(int x, int y, int w, int h)
{
	DFBRectangle rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	CHECK(m_surf->Blit(m_surf, m_surf2, &rect, m_x, m_y));
	CHECK(m_surf->Flip(m_surf, NULL, DSFLIP_NONE));
out:
    return;
}

void RunBlit(void)
{
	int i = 0;
	int textisruning = 1;
	while(textisruning){
		i = i + 1;
		if (i >= 350){	
			textisruning = 0; 				
			break;
		}
		BlitSurface(0, 0, i, 150);
		//usleep(50000);
	}
	if (m_surf2)	
		m_surf2->Release(m_surf2);
	if (m_wnd2)
		m_wnd2->Release(m_wnd2);

	//printf("\n\n\033[31m FINISH COLOR TEXT - SCTV -\033[0m\n\n");

}
void runloading();
void ShowLoading();
int getstatusLoading();
int nRun = 0;
int NUM_LOADING = 6;
void *run_text_thread(void* x)
{
	//RunBlit();
	if (nRun == 1)
		return NULL;
	runloading();
	return NULL;
}
typedef struct Rectangle{
	int x;
	int y;
	int w;
	int h;
	int color_a;
	int color_r;
	int color_g;
	int color_b;
}SRectangle;

SRectangle s_ArrRec[] = { //369cf1
	{544, 505, 13, 13, 0xff, 0x19, 0x5f, 0x9c},
	{580, 505, 13, 13, 0xff, 0x19, 0x5f, 0x9c},
	{615, 505, 13, 13, 0xff, 0x19, 0x5f, 0x9c},
	{652, 505, 13, 13, 0xff, 0x19, 0x5f, 0x9c},
	{688, 505, 13, 13, 0xff, 0x19, 0x5f, 0x9c},
	{723, 505, 13, 13, 0xff, 0x19, 0x5f, 0x9c}	
};

void runloading(void)
{
	DFBInputDeviceKeySymbol mKey = DIKS_ENTER;
	//echo 1 > /tmp/loading.txt to exit while
	while (getstatusLoading() == 0){
		usleep(200000);
	}
	sawman_post_key_event(mKey);
}
void runloading2(void);
void runloading2(void){
	if (nRun == 1)
		return;
	int i = 0;
	nRun = 1;
	DFBRectangle DFBrect;
	DFBrect.x = 0;
	DFBrect.y = 0;
	DFBrect.w = 1280;
	DFBrect.h = 720;
	if ( load_image((char*)"/tango/app/images/SCTV_Logo_Booting.png", DFBrect) == DFB_OK) {	   
		m_surf->Blit(m_surf, slidesurface, NULL, 0,0);   
		m_surf->SetColor(m_surf, 0xbb, 0xbb, 0xbb, 0xff);	
		for (int rec = 0; rec < NUM_LOADING; rec ++){
			m_surf->FillRectangle(m_surf, s_ArrRec[rec].x, s_ArrRec[rec].y, s_ArrRec[rec].w, s_ArrRec[rec].h);
		}
		m_surf->Flip( m_surf, NULL, (DFBSurfaceFlipFlags) 0 );
	} else {		
		 //printf("\n  ---- load image error ----\n");
	}
	CHECK(m_layer->GetSurface(m_layer, &m_surf));
	if (!m_layer)
	{
		memset(&m_layer, 0, sizeof(m_layer));
		CHECK(gCtx.pDfb->GetDisplayLayer(gCtx.pDfb, EM86LAYER_SECOSD, &m_layer));
		CHECK(m_layer->SetCooperativeLevel(m_layer, DLSCL_EXCLUSIVE));
	}
	while (getstatusLoading() == 0){		
		m_surf->SetColor(m_surf, 0xbb, 0xbb, 0xbb, 0xff);	
		for (int rec = 0; rec < NUM_LOADING; rec ++){
			m_surf->FillRectangle(m_surf, s_ArrRec[rec].x, s_ArrRec[rec].y, s_ArrRec[rec].w, s_ArrRec[rec].h);
		}
		m_surf->SetColor(m_surf, s_ArrRec[i].color_r, s_ArrRec[i].color_g, s_ArrRec[i].color_b, s_ArrRec[i].color_a);	
		m_surf->FillRectangle(m_surf, s_ArrRec[i].x, s_ArrRec[i].y, s_ArrRec[i].w, s_ArrRec[i].h);
		m_surf->Flip(m_surf, NULL, DSFLIP_NONE);
		m_wnd->SetOpacity(m_wnd, 0xFF);   // show window
		i = i + 1;
		if (i == NUM_LOADING){
			i = 0;
		}
		//usleep(500000);		
		usleep(200000);		
	}	
out:
	m_surf->Clear(m_surf, 0, 0, 0, 0);    
    if (slidesurface) {
		slidesurface->Clear( slidesurface, 0, 0, 0, 0); 
	}	
	if (m_layer) {
		m_layer->Release(m_layer);
		m_layer = NULL;
	}	
	m_wnd->SetOpacity(m_wnd, 0x0);
}

void ShowLoading()
{
	pthread_t loadingthread;
	pthread_create(&loadingthread, NULL,  run_text_thread, NULL);
	
}

int getstatusLoading()
{
	FILE *f = NULL;		
	f = fopen("/tmp/loading.txt", "rb");
	if(f == NULL) 
	{ 
		return 0; 
	}
	char str_tmp[2];
	if (fgets(str_tmp, 2, f) != NULL){
		return atoi(str_tmp);
	}
	return 0;	
}
DFBResult GuiInitialize( IDirectFB* dfb )
{    
    int width = 0; 
    int height = 0;

    CHECK(dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &m_layer));
    //CHECK(m_layer->SetCooperativeLevel(m_layer, DLSCL_ADMINISTRATIVE));

    // Get screen size
    // We are taking the osd screen size
    CHECK(dfb->GetScreen(dfb, 0, &dfb_osd_screen));
    CHECK(dfb_osd_screen->GetSize(dfb_osd_screen, &width, &height));

    // set the drawing pos by the report of the screen 
    //width_raport = m_app_man->m_screen_width / 1920.0;
    //drawing_pos_x = DRAWING_POS_X * width_raport;
    //drawing_pos_y = DRAWING_POS_Y * m_app_man->m_screen_height / 1080;
    //menu_separator_width = 10 * width_raport;

    DFBWindowDescription wdsc;

    wdsc.flags  = (DFBWindowDescriptionFlags)( DWDESC_POSX | DWDESC_POSY |
                  DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS );
    wdsc.posx   = 0;
    wdsc.posy   = 0;
    wdsc.width  = width;
    wdsc.height = height;
    wdsc.caps   = DWCAPS_NODECORATION;

    CHECK(m_layer->CreateWindow(m_layer, &wdsc, &m_wnd));
    CHECK(m_wnd->SetOpacity(m_wnd, 0xff));
    CHECK(m_wnd->SetStackingClass(m_wnd, DWSC_UPPER));

    CHECK(m_wnd->GetSurface(m_wnd, &m_surf));
	
#if 0 //create windown 2
	//color
	DFBWindowDescription wdsc_color;

    wdsc_color.flags  = (DFBWindowDescriptionFlags)( DWDESC_POSX | DWDESC_POSY |
                  DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS );
    wdsc_color.posx   = 450;
    wdsc_color.posy   = 280 + 150;
    wdsc_color.width  = width/2;
    wdsc_color.height = height;
    wdsc_color.caps   = DWCAPS_NODECORATION;
    CHECK(m_layer->CreateWindow(m_layer, &wdsc_color, &m_wnd2));
   //CHECK(m_wnd2->SetOpacity(m_wnd2, 0xff)); //do not show in screen
    CHECK(m_wnd2->SetStackingClass(m_wnd2, DWSC_UPPER));
    CHECK(m_wnd2->GetSurface(m_wnd2, &m_surf2));
#endif
#if 0 // show text
    // set up font
    char font_filename[1024];
	if (FONTNAME[0] != '/')
	{
		char *dfbDir = getenv("DCCHD_INSTALL_DIR");
		if (dfbDir)
		{
			strcpy(font_filename, dfbDir);
			strcat(font_filename, "/share/directfb-examples/fonts/");
		}
	}
	strcat(font_filename, FONTNAME);

	DFBFontDescription fdsc;
	fdsc.flags = DFDESC_HEIGHT;
	fdsc.height = 150;
   CHECK(dfb->CreateFont(dfb, font_filename, &fdsc, &m_font_big));

   //66FFFF
    m_surf->SetColor(m_surf, 0x66, 0xFF, 0xFF, 0xff);
    m_surf->SetFont(m_surf, m_font_big);
    m_surf->DrawString(m_surf, "SCTV", -1, 450, 280, DSTF_TOPLEFT);
	

	 m_surf2->SetFont(m_surf2, m_font_big);

	 //red
	m_surf2->SetColor(m_surf2, 0xFF, 0x0, 0x0, 0xff);
    m_surf2->DrawString(m_surf2, "S", -1, 0, 0, DSTF_TOPLEFT);
	
	//green
	m_surf2->SetColor(m_surf2, 0x0, 0xFF, 0x0, 0xFF);
    m_surf2->DrawString(m_surf2, "C", -1, 70, 0, DSTF_TOPLEFT);
	
	//yellow: FFFF00
	m_surf2->SetColor(m_surf2, 0xFF, 0xFF, 0x00, 0xff);
    m_surf2->DrawString(m_surf2, "T", -1, 185, 0, DSTF_TOPLEFT);
	
	//blue
	m_surf2->SetColor(m_surf2, 0x00, 0x0, 0xFF, 0xff);
    m_surf2->DrawString(m_surf2, "V", -1, 240, 0, DSTF_TOPLEFT);
	
    m_surf->Flip(m_surf, NULL, DSFLIP_NONE);
    m_surf2->Flip(m_surf2, NULL, DSFLIP_NONE);
	sleep(3);
	//printf("\n\033[31mWait for result\n");
	RunBlit();
#else	// show image
	/*
	DFBRectangle DFBrect;
	DFBrect.x = 0;
	DFBrect.y = 0;
	DFBrect.w = 1280;
	DFBrect.h = 720;
	if ( load_image((char*)"/tango/app/images/SCTV_Logo_Booting.png", DFBrect) == DFB_OK) {	   
		m_surf->Blit(m_surf, slidesurface, NULL, 0,0);   
		m_surf->SetColor(m_surf, 0xbb, 0xbb, 0xbb, 0xff);	
		for (int rec = 0; rec < NUM_LOADING; rec ++){
			m_surf->FillRectangle(m_surf, s_ArrRec[rec].x, s_ArrRec[rec].y, s_ArrRec[rec].w, s_ArrRec[rec].h);
		}
		m_surf->Flip( m_surf, NULL, (DFBSurfaceFlipFlags) 0 );
	} else {		
		 //printf("\n  ---- load image error ----\n");
	}
	*/
	ShowLoading();
	
#endif	
    return DFB_OK;

out:
    return DFB_FAILURE;
}

#endif

int main(int argc, const char** argv)
{	

#ifdef POWER_CONTROL
	if ( (r = mum_init( 0, &pLLAD, &pGBus)) != RM_OK )
	{
		MPRINT(("main !mum_init FAILED %d\ncheck your 86xx driver\n", r));
		exit(r);
	}

	/* I'm only going to use 1 I2c bus, but you can have multiple linked to different tuners */
	memset(&i2cConfig, 0, sizeof(struct i2c) );
	i2cConfig.pGBus = (gbus*) pGBus;
	i2cConfig.PioClock = 0;
	i2cConfig.PioData = 1;
	i2cConfig.RegBase = REG_BASE_system_block;
	i2cConfig.DelayUs = 0x8000;	 // for emi2c, this will use default, for i2c_hal, set to 10

	pI2Cal_Context = (I2CAL_CONTEXT*)&i2cConfig;

	ucTunerByte[0] = 0x30;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x10, ucTunerByte, &size);

	ucTunerByte[0] = 0xe0;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x20, ucTunerByte, &size);

	ucTunerByte[0] = 0x81;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x21, ucTunerByte, &size);

	ucTunerByte[0] = 0xa0;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x30, ucTunerByte, &size);

	ucTunerByte[0] = 0xb5;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x32, ucTunerByte, &size);
	ucTunerByte[0] = 0x4a;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x33, ucTunerByte, &size);
	ucTunerByte[0] = 0xcb;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x34, ucTunerByte, &size);
	ucTunerByte[0] = 0x04;
	I2Cal_Write_Subaddress(pI2Cal_Context, 0x68, 0x35, ucTunerByte, &size);
#endif
   memset(&gCtx, 0, sizeof(gCtx));	
   pid_t pid = 0;

   static SaWManCallbacks callbacks;

   callbacks.Start                = start_request;
   callbacks.Stop                 = stop_request;
   callbacks.ProcessAdded         = process_added;
   callbacks.ProcessRemoved       = process_removed;
   callbacks.InputFilter          = input_filter;
   callbacks.WindowPreConfig      = window_preconfig;
   callbacks.WindowAdded          = window_added;
   callbacks.WindowRemoved        = window_removed;
   callbacks.WindowReconfig       = window_reconfig;
   callbacks.WindowRestack        = window_restack;
   //~ StackResized
   callbacks.SwitchFocus          = switch_focus;
   //~ LayerReconfig
   callbacks.ApplicationIDChanged = application_id_changed;

   D_INFO("SaWMan/AppLauncher: Initializing...\n");

   CHECK(DirectFBInit(&argc, (char ***)&argv));

   CHECK(DirectFBCreate(&gCtx.pDfb));

   CHECK(SaWManCreate(&gCtx.pSaw));

   CHECK(gCtx.pSaw->CreateManager(gCtx.pSaw, &callbacks, &gCtx, &gCtx.pManager));

   gCtx.pLayout = new FullScreenLayout( gCtx.pManager );


   // open to show some text
   nRun = 0;
   CHECK(GuiInitialize(gCtx.pDfb));	   

   // Create a fifo to read commands on:
   if( mkfifo(SYSTEM_FIFO_NAME, 0600) < 0 )
   {
      if( EEXIST != errno )
      {
         D_INFO("Failed to create command fifo.\n");
         perror("mkfifo");
         goto out;
      }
   }

   if( (gCtx.fifo = open(SYSTEM_FIFO_NAME, O_RDONLY | O_NONBLOCK)) < 0 )
   {
      D_INFO("Failed to open command fifo.\n");
      perror("open");
      goto out;
   }

   // Startup the launcher UI
   
   CHECK(gCtx.pSaw->Start(gCtx.pSaw, "ui", &pid));
   //sleep(10);
	/*
   m_surf->Clear(m_surf, 0, 0, 0, 0);
     
    if (slidesurface) {
		slidesurface->Clear( slidesurface, 0, 0, 0, 0); 
	}
	
	if (m_layer) {
		m_layer->Release(m_layer);
		m_layer = NULL;
	}
	
	CHECK(m_wnd->SetOpacity(m_wnd, 0x0));
	*/
   while( true != gbExit ) 
   {
#ifdef LAUNCHER_DEBUG
      struct timeval timeo;
      fd_set         rset;
      int            ch;
      int            rc = -1;
      
      FD_ZERO( &rset );
      FD_SET( 0, &rset );

      timeo.tv_sec = 1;
      timeo.tv_usec = 0;

      rc = select( 1, &rset, NULL, NULL, &timeo );

      switch( rc )
      {
         case -1:
            if(( EINTR != errno ) && ( EAGAIN != errno ))
            {
               D_INFO( "SaWMan/AppLauncher: Error reading input\n" );
            }
            continue;
            
         default:
            if( FD_ISSET( 0, &rset ))
            {
               ch = getchar();
               if( 'q' == ch )
               {
                  gbExit = true;
                  continue;
               }
            }
            break;
      }
#else
      // Do something usefull one day :)

      sleep(1);
#endif
	
   }

out:
   D_INFO("SaWMan/AppLauncher: Shutting down...\n");
   gbExit = true;

   if( gCtx.pLayout )
      delete  gCtx.pLayout;

   if(gCtx.pManager)
      gCtx.pManager->Release(gCtx.pManager);

   if(gCtx.pSaw)
      gCtx.pSaw->Release(gCtx.pSaw);

   if( gCtx.pDfb)
      gCtx.pDfb->Release(gCtx.pDfb);

   return 0;
}

