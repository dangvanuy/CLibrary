#include<stdio.h>
#include<signal.h>
#include<unistd.h>

/*
Termination Signals:
1. SIGTERM:
	The SIGTERM signal is a generic signal used to cause program termination. Unlike SIGKILL, this signal can be blocked, handled, and ignored. It is the normal way to politely ask a program to terminate.
	The shell command kill generates SIGTERM by default.
2. SIGINT
	The SIGINT (“program interrupt”) signal is sent when the user types the INTR character (normally C-c). See Special Characters, for information about terminal driver support for C-c.
3. SIGQUIT
	The SIGQUIT signal is similar to SIGINT, except that it's controlled by a different key—the QUIT character, usually C-\—and produces a core dump when it terminates the process, just like a program error signal. You can think of this as a program error condition “detected” by the user
4. SIGKILL
	The SIGKILL signal is used to cause immediate program termination. It cannot be handled or ignored, and is therefore always fatal. It is also not possible to block this signal.
5. SIGHUP
	The SIGHUP (“hang-up”) signal is used to report that the user's terminal is disconnected, perhaps because a network or telephone connection was broken. For more information about this, see Control Modes.
*/

int countExit = 4;
void sig_handler(int signo)
{
	if (signo == SIGINT){

		countExit -= 1;
		if (countExit == 0){
			printf("received SIGINT, count down exit: Exited\n");
		}else{
			printf("received SIGINT, count down exit: %d\n", countExit);
		}
	}else if (signo == SIGQUIT){
		printf("received signal SIGQUIT\n");
	}else if (signo == SIGKILL){
		printf("received signal SIGKILL\n");
		countExit = 0;
	}
}

int main(int agrc, char* agrv[])
{
	if (signal(SIGINT, sig_handler) == SIG_ERR){
		printf("\ncan't catch SIGINT\n");
	}else {
		printf("Press ctrl + c 3 times to exit(send signal interrupt to main thread)\n");
	}

	if (signal( SIGQUIT, sig_handler ) == SIG_ERR){
		printf("\ncan't catch SIGQUIT\n");
	}else {
		printf("How to send signal SIGQUIT to main thread\n");
	}

	if (signal( SIGKILL, sig_handler ) == SIG_ERR){
			printf("\ncan't catch SIGKILL\n");
		}else {
			printf("To send signal SIGKILL: please use cmd: killall -9 %s\n", agrv[0]);
		}
	// A long long wait so that we can easily issue a signal to this process
	while(countExit){
		sleep(1);
	}
	return 0;
}
