#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pthread.h> //for thread
#include <fcntl.h>

//#include "emhwlib_hal/include/emhwlib_registers.h"
#include "llad/include/gbus.h"
#include "tuner/helper/helper.h"

#include "directfb.h"
#include "remote.h"

#define SMP_REMOTE_DEV "/dev/ir"
unsigned flags = O_RDONLY;
int fno;
fd_set rfds;
struct timeval tv;
int ret;
void (*pFUNC_Global_Handle)(unsigned long) = NULL;

/*
void (*pFUNC_callbackmain)(int) = NULL;
void initCallBackMain(void *pFunc )
{
	pFUNC_callbackmain = pFunc;
}
void call_func_from_main()
{
	pFUNC_callbackmain();
}
*/
void *smp_remote_event_thread(void *n)
{
    union {
    unsigned long l;
    unsigned char c[sizeof(unsigned long)];
    }u;
    int cnt = 0;
	while(1){
		if (flags & O_NONBLOCK) {
            if (read(fno, &u.l, sizeof(unsigned long)) > 0)
                printf("A. Iter(%d): 0x%08lx\n", cnt, u.l);
            else
                printf("A. Iter(%d): nothing >0x%08lx<\n", cnt, u.l);
        } else {
            fd_set rfds;
            struct timeval tv;
            int ret;

            FD_ZERO(&rfds);
            FD_SET(fno, &rfds);
            tv.tv_sec = 5;
            tv.tv_usec = 0;

            ret = select(fno + 1, &rfds, NULL, NULL, &tv);
            if (ret) {
                if (read(fno, &u.l, sizeof(unsigned long)) > 0){
					printf("\n==============================================\n");
					if (pFUNC_Global_Handle != NULL){
						pFUNC_Global_Handle(u.l); //for gui
					} else {
						printf("pFUNC_Global_Handle IS NULL\n");
					}
                }else{
                    printf("B. Iter(%d): nothing >0x%08lx<\n", cnt, u.l);
				}
            } else {
				//printf("B. Iter(%d): timeout\n", cnt);
            }
        }
        cnt += 1;
        if (u.l == Key_POWER){ //key power
            break;
        }
	}
	printf("\nGui is stoped to receive Key!\n");
	return NULL;
}

pthread_t var_thread_remote;
int driver_open_device() {

	int status = pthread_create(&var_thread_remote, NULL, smp_remote_event_thread, NULL);
	
	return status;
}


void run_remote()
{
	if ((fno = open(SMP_REMOTE_DEV, flags)) < 0) {
		printf("error opening %s\n", SMP_REMOTE_DEV);
		printf("You have run cmd: \"mknod %s c 253 0\" yet?\n", SMP_REMOTE_DEV);
		return;
	}
	
    FD_ZERO(&rfds);
    FD_SET(fno, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 500000; /* 0.5 second */

    ret = select(fno + 1, &rfds, NULL, NULL, &tv);
    driver_open_device();    
}

void stop_remote()
{
	pthread_join(var_thread_remote, NULL);
	close(fno);
	printf("Close remote\n");
}

/*//////////////////////////////// directfb */
extern int 	keyfd;
void *smp_remote_event_thread_dfb(void *n)
{
	printf("keyfd = %d\n", keyfd);
	while(1){		
		DFBEvent evt;
        //Initialize the file descriptor set. 
        fd_set set;
        
        FD_ZERO (&set);
        FD_SET (keyfd, &set);
        // select returns 0 if timeout, 1 if input available, -1 if error. 
        struct timeval timeout = {0, 100000};
		memset(&evt,0,sizeof(evt));
        select (FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (FD_ISSET(keyfd, &set))
		{
			read(keyfd, &evt, sizeof(evt));
			//evt = get_event();
			
			
			if ( evt.clazz == DFEC_INPUT )
			{
				if (pFUNC_Global_Handle != NULL ){
					if (evt.input.type != DIET_KEYRELEASE){
						if (evt.input.key_symbol == Key_POWER){
							printf("Power Off >> good bye\n");
							break;
						}
						printf("Press key: %u\n", evt.input.key_symbol);
						pFUNC_Global_Handle(evt.input.key_symbol);
					}
				} else {
					printf("pFUNC_Global_Handle IS NULL\n");
				}
			}
		}
	}
	printf("\nThread is exit");
	return NULL;
}

pthread_t var_thread_dfb;
void run_remote_dfb()
{
	printf(">>>>>>>>>>>>>>> Start run_remote_dfb\n");
	pthread_create(&var_thread_dfb, NULL, smp_remote_event_thread_dfb, NULL);
}

void stop_remote_dfb()
{
	pthread_join(var_thread_dfb, NULL);
	printf("Close remote\n");
}

void set_handle(void *handle)
{
	pFUNC_Global_Handle = handle;
}








