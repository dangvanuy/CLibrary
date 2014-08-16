#include <stdio.h>
#include <unistd.h>

extern "C"{
#include "dtv_cmd.h"
#include "dtv/media_dtv.h"
#include "media.h"
#include "init_directfb.h"
}

#include "remote.h"

/*
* control thread via remote from user
*/
int icount = 0;
void HandleMain(unsigned long key)
{
	printf("\n\n");
	icount += 1;
	switch (key) {
		case Key_DOWN:
			printf("%d. GUI >> Down sleep(1s)\n", icount);
			//sleep(1);
			break;
		case Key_LEFT:
			printf("%d. GUI >> Left sleep(2s)\n", icount);
			//sleep(2);
			printf("%d. GUI >> Left sleep(2s) done\n", icount);
			break;
		case Key_RIGHT:
			printf("%d. GUI >> Right sleep(3s)\n", icount);
			//sleep(3);
			printf("%d. GUI >> Right sleep(3s) done\n", icount);
			break;
		case Key_UP:
			printf("%d. GUI >> Up sleep(4s)\n", icount);
			//sleep(4);
			printf("%d. GUI >> Up sleep(4s) done\n", icount);
			break;
		case Key_ENTER:
			printf("%d. GUI >> Enter no sleep\n", icount);
			break;
		case Key_ZERO:
			printf("%d. GUI >> Zero sleep(2) \n", icount);
			//sleep(2);
			break;
		case Key_ONE:
			printf("%d. GUI >> ONE sleep(1sec)\n", icount);
			//sleep(1);
			printf("%d. GUI >> ONE sleep(1sec) done\n", icount);
			//sleep(1);
			break;
		default:
			printf("%d. GUI >> Press key 0x%08lx\n", icount, key);
			break;
	}

}

int main()
{
	global_init(0, 0);

	printf("Main App\n");
	//init remote
	set_handle((void*)HandleMain);
	run_remote_dfb();

	sleep(2);
	printf("Begin autosendkey\n");
	post_event(Key_DOWN);
	//post_event(Key_LEFT);
	//post_event(Key_RIGHT);
	//post_event(Key_ENTER);

	stop_remote_dfb();
	return 0;
}

