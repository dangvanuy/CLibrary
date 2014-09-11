/*
   (c) Copyright 2010 Sigma Designs

   Proprietary and confidential.
   */

#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <directfb.h>
#include <direct/messages.h>
#include <direct/list.h>
#include <sawman.h>

#include "app.h"


static DirectLink* gAppList = NULL;


/**
 * Search the list of applications for a match on 'pid'.  There should be at most one match in the
 * list. 
 *
 * \param[in] pid
 *            The process id to search for.
 *
 * \return The address of the application structure matching 'pid' or NULL.
 */
app_t* app_find( pid_t pid )
{
   DirectLink* elem = NULL;
   app_t*      pApp = NULL;
   
   direct_list_foreach( elem, gAppList )
   {
      if( ((app_t*)elem)->pid == pid )
      {
         pApp = (app_t*)elem;
         break;
      }
   }

   return pApp;
}


/**
 * Add an application structure to the list for 'pid'.  There should be at most one entry in the
 * list for a given 'pid' value, so if an existing entry already exists, no structure is added.
 *
 * \param[in] pid
 *            The process id to add.
 * \param[in] pName
 *            Name of the application.
 *            
 * \return The address of the added or existing structure, or NULL on error.
 */
app_t* app_add( pid_t pid, const char* pName )
{
   app_t* pApp = NULL;

   /* If no existing entry then add one */
   pApp = app_find( pid );
   
   if( NULL == pApp )
   {
      pApp = (app_t*)calloc( 1, sizeof( app_t ));
      if( NULL != pApp )
      {
         if( 0 != pthread_mutex_init( &pApp->mutex, NULL ))
         {
            D_INFO( "Error creating application mutex\n" );
         }
         pApp->pid     = pid;
         pApp->pName   = (NULL!=pName)?strdup( pName ):NULL;
         pApp->pWmProc = NULL;
         pApp->bWaited = false;
         direct_list_append( &gAppList, &pApp->link );
         D_INFO( "Added application \"%s\" pid %d\n", pApp->pName, pApp->pid );
      }
   }

   return pApp;
}


/**
 * Remove an application structure to the list for 'pid'.  There should be at most one entry in the
 * list for a given 'pid' value..
 *
 * \param[in] pid
 *            The process id to remove.
 */
void app_remove( pid_t pid )
{
	printf("\n--VAN LE: app.cpp --> app_remove()\n\n");
   DirectLink* elem = NULL;
   DirectLink* temp = NULL;
   
   direct_list_foreach_safe( elem, temp, gAppList )
   {
		printf("\n--VAN LE: app.cpp --> app_remove() --> direct_list_foreach_safe\n\n");
      app_t* pApp = (app_t*)elem;
      if( pApp->pid == pid )
      {
         direct_list_remove( &gAppList, &pApp->link );
         if( NULL != pApp->pName )
         {
            free( pApp->pName );
         }
         if( 0 != pthread_mutex_destroy( &pApp->mutex ))
         {
            D_INFO( "Error destroying application mutex\n" );
         }
         free( pApp );

         D_INFO( "Removed application pid %d\n", pid );

         /* Should only be one entry for 'pid' */
         break;
      }
   }
	printf("\n--VAN LE: app.cpp --> app_remove() --> end\n\n");
   return;
}


/**
 * Blocking call to acquire the app's mutex.
 *
 * \param[in] pApp
 *            The appliction to acquire the mutex for.
 *
 * \retval 0 Successfully obtained the lock.
 * \retval non-zero Error.
 */
int app_lock( app_t* pApp )
{
   int rc = -1;
   
   if( NULL != pApp )
   {
      rc = pthread_mutex_lock( &pApp->mutex );
   }

   return rc;
}


/**
 * Non-blocking call to acquire the app's mutex.
 *
 * \param[in] pApp
 *            The appliction to acquire the mutex for.
 *
 * \retval 0 Successfully obtained the lock.
 * \retval non-zero Could not obtain the lock, or error.
 */
int app_try_lock( app_t* pApp )
{
   int rc = -1;
   
   if( NULL != pApp )
   {
      rc = pthread_mutex_trylock( &pApp->mutex );
   }

   return rc;
}


/**
 * Unlock the applications mutex.
 *
 * \param[in] pApp
 *            The appliction to release the mutex for.
 *
 * \retval 0 Successfully released the lock.
 * \retval non-zero error
 */
int app_unlock( app_t* pApp )
{
   int rc = -1;
   
   if( NULL != pApp )
   {
      rc = pthread_mutex_unlock( &pApp->mutex );
   }

   return rc;
}
