#include <stdio.h>
#include <string.h> //for memset
#include <stdlib.h> //for malloc
#include <unistd.h> //for usleep
#include <pthread.h> //for thread

void * test_threadDFB_proc(DFBThread *thread, void *ptr)
{
	printf("This is test  thread of DFB\n");
	return NULL;
}

public class CTestThread
{

}
int main()
{	
	printf("Main\n");
	int thread_id = DFBCreateThread(DTT_DEFAULT, (DirectThreadMainFunc)test_threadDFB_proc, NULL, "test_threadDFB_proc");
	return 0;
}
//g++ -o main main.cpp -lpthread
