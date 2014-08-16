#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for pipe
#include <string.h>

int main(int argc, char *argv[])
{
   int pipefd[2];
   char buf;
   char data[100];

	if (argc != 2) {
		printf("Usage: %s <string>\n", argv[0]);
		return 0;
	}

	if (pipe(pipefd) == -1) {
		printf("Can not create pipe\n");
		return 0;
	}
	
	//write data to pipe
	//close(pipefd[0]);
	write(pipefd[1], argv[1], strlen(argv[1]));
	
	
	//read data from pipe
	close(pipefd[1]);
	int i = 0;
	while ((read(pipefd[0], &buf, 1) > 0) && (i < 100)){
		printf("[%d]read and write buf: %c\n", i, buf);	  
		data[i++] = buf;
		if (i == 99)
			break;
	}
	data[i] = '\0';
	
	printf("Data were readed from pipe: %s\n", data);

	printf("Close pipe\n");
	close(pipefd[1]);
	close(pipefd[0]);
}
//gcc -o pipe_hello_world pipe_hello_world.c


