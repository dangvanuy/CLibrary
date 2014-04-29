// mutex example

#include <stdio.h>
#include <unistd.h> //sleep
#include <stdlib.h> //malloc
#include <pthread.h>

typedef struct SArg
{
	int n;
	char c;
}argData;
void *thread(void *pObj)
{
	argData *m_pObj = (argData*)pObj;
	int i;
	for (i = 0; i < m_pObj->n; i++){
		printf(" %d%c ", i, m_pObj->c);
		usleep(500);
	}
	printf("\n");
	return NULL;
}

pthread_t threadID[10];
int IndexThreadID = 0;
void Run(int n, int c){
	//argData arg;
	//arg.c = c;   Error when pass for thread
	//arg.n = n;
	
	argData *arg = (argData*)malloc(sizeof(argData*)); //pass for thread ok
	arg->c = c;
	arg->n = n;
	
	pthread_create(&threadID[IndexThreadID], NULL, thread, arg);	
	IndexThreadID += 1;
}

int main ()
{

  Run(50, 'a');
  Run(50, 'b');
  Run(50, 'c');
  Run(50, 'd');

#if 1 
  for (int i = 0; i < IndexThreadID; i++){
	pthread_join(threadID[i], NULL);
  }
#endif

  //sleep(1);
  exit(0);
}
