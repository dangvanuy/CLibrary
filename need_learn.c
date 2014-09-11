need learn and write a sample

1.struct sigaction //support by kernel
{
	Header  : #include <signal.h>
	Mục đích:
	Ý nghĩa :
	Docs	:{
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

	}
	sample 1:{
		//http://www.linuxprogrammingblog.com/code-examples/sigaction
		/* Example of using sigaction() to setup a signal handler with 3 arguments
		 * including siginfo_t.
		 */
		#include <stdio.h>
		#include <unistd.h>
		#include <signal.h>
		#include <string.h>
		 
		static void hdl (int sig, siginfo_t *siginfo, void *context)
		{
			printf ("Sending PID: %ld, UID: %ld\n",
					(long)siginfo->si_pid, (long)siginfo->si_uid);
		}		 
		int main (int argc, char *argv[])
		{
			struct sigaction act;		 
			memset (&act, '\0', sizeof(act));
		 
			/* Use the sa_sigaction field because the handles has two additional parameters */
			act.sa_sigaction = &hdl;
		 
			/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
			act.sa_flags = SA_SIGINFO;
		 
			if (sigaction(SIGTERM, &act, NULL) < 0) {
				perror ("sigaction");
				return 1;
			}	
			while (1)
				sleep (10);		 
			return 0;
		}
	}
	sample 2: sigmacode/smpoutd.c {
		static void local_catch_signals(void)
		{
			struct sigaction action;
			struct sigaction crash;

			action.sa_handler = local_signal_handler;
			action.sa_flags = 0;
			sigemptyset(&action.sa_mask);
			sigaction(SIGINT, &action, NULL);
			sigaction(SIGTERM, &action, NULL);

			crash.sa_handler = local_crash_handler;
			crash.sa_flags = 0;
			sigemptyset(&crash.sa_mask);
			sigaction(SIGBUS, &crash, NULL);
			sigaction(SIGABRT, &crash, NULL);
			sigaction(SIGSEGV, &crash, NULL);
			sigaction(SIGILL, &crash, NULL);
		}
	}
}

2. ioctl //io controler
{
	Header: #include <sys/ioctl.h>
	Docs  :	http://man7.org/linux/man-pages/man2/ioctl.2.html
}

3. mmap
{
	- header: #include <sys/mman.h>
	- Docs: 
		http://pubs.opengroup.org/onlinepubs/009695399/functions/mmap.html
		http://en.wikipedia.org/wiki/Memory-mapped_file
	- example:
		http://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/
}


4. select
{
}












