//docs: http://man7.org/linux/man-pages/man2/pipe.2.html

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for pipe
#include <string.h>

int main(int argc, char *argv[])
{
   int pipefd[2];
   pid_t cpid;
   char buf;

   if (argc != 2) {
	   fprintf(stderr, "Usage: %s <string>\n", argv[0]);
	   exit(EXIT_FAILURE);
   }

   if (pipe(pipefd) == -1) {
	   perror("pipe");
	   exit(EXIT_FAILURE);
   }

   cpid = fork();
   if (cpid == -1) {
	   perror("fork");
	   exit(EXIT_FAILURE);
   }else {
	printf("cpid = %d\n", cpid);
   }

   if (cpid == 0) {    /* Child reads from pipe */
	   close(pipefd[1]);          /* Close unused write end */

	   while (read(pipefd[0], &buf, 1) > 0){
			printf("read and write buf: %c\n", buf);
		   write(STDOUT_FILENO, &buf, 1);
		}
	   write(STDOUT_FILENO, "\n", 1);
	   close(pipefd[0]);
	   _exit(EXIT_SUCCESS);

   } else {            /* Parent writes argv[1] to pipe */
	   close(pipefd[0]);          /* Close unused read end */
	   write(pipefd[1], argv[1], strlen(argv[1]));
	   printf("Write data into pipe: %s\n",  argv[1]);
	   close(pipefd[1]);          /* Reader will see EOF */
	   wait(NULL);                /* Wait for child */
	   exit(EXIT_SUCCESS);
   }
}
//gcc -o pipe_hello_world pipe_hello_world.c


