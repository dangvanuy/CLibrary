/**
 * Common defines used by both the UI and launchd
 *
 *
 */
#ifndef __COMMON_H__
#define __COMMON_H__

extern "C" {
#define SYSTEM_FIFO_NAME   "/tmp/launchd"

#define SYSTEM_MAX_CMD_LEN (5*1024) 

#define COMMAND_MAX_ARGS   (20)
#define COMMAND_MAX_ENVS   (20)

#define LOG_ERROR( X... ) fprintf(stderr, "(!) " X)

#define TRACE( ) printf("TRACE => %s:%d\n", __FILE__, __LINE__)

typedef enum
{
   COMMAND_INVALID = -1,
   COMMAND_QUIT = 0,
   COMMAND_APP = 1

} command_type_t;


/**
 * Used to store env variables to set
 */
typedef struct 
{
   int    bReplace;
   char*  name;
   char*  value;

} command_env_t;

/**
 * Command structure
 */
typedef struct
{
   command_type_t  cmd;
   char*           name;
   char*           app;
   char*           path;
   int             num_args;
   int             num_envs;
   char*           args[COMMAND_MAX_ARGS + 2]; // Add two here to allow NULL to be the last arg and the app to be the first arg
   command_env_t   envs[COMMAND_MAX_ENVS]; 

} command_t;

}
#endif

