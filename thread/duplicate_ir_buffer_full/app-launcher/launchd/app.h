/*
   (c) Copyright 2010 Sigma Designs

   Proprietary and confidential.
   */

typedef struct
{
   DirectLink      link;
   pthread_mutex_t mutex;
   pid_t           pid;
   char*           pName;
   SaWManProcess*  pWmProc;
   bool            bWaited;
} app_t;


app_t*  app_find( pid_t );
app_t*  app_add( pid_t, const char* );
void    app_remove( pid_t );
int     app_lock( app_t* );
int     app_try_lock( app_t* );
int     app_unlock( app_t* );
