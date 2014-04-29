#include <stdio.h>
#include <unistd.h>
#include "remote.h"
/*
* control thread via remote from user
*/
int icount = 0;
void HandleMain(unsigned long key)
{
	icount += 1;
	switch (key) {
		case Key_DOWN:
			printf("%d. GUI >> Down sleep(1s)\n", icount);
			sleep(1);
			break;
		case Key_LEFT:
			printf("%d. GUI >> Left sleep(2s)\n", icount);
			sleep(2);
			break;
		case Key_RIGHT:
			printf("%d. GUI >> Right sleep(3s)\n", icount);
			sleep(3);
			break;
		case Key_UP:
			printf("%d. GUI >> Up sleep(4s)\n", icount);
			sleep(4);
			break;
		case Key_ENTER:
			printf("%d. GUI >> Enter no sleep\n", icount);
			break;
		case Key_ZERO:
			printf("%d. GUI >> ZERO no sleep\n", icount);
			break;
		case Key_ONE:
			printf("%d. GUI >> ONE sleep(1sec)\n", icount);
			sleep(1);
			break;
		default:
			printf("%d. GUI >> Press key 0x%08lx\n", icount, key);
			break;
	}

}

int main()
{
	printf("Main App\n");
	//init remote
	set_handle((void*)HandleMain);
	run_remote();	
	
	stop_remote();
	return 0;
}

