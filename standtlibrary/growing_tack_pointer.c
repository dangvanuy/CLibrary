#include <stdio.h>

#if 0
int main () 
{ 
	printf("Test Stack pointer\n");
	return 0;
}
#else
volatile int *a = 0, *b = 0;
void foo (void);
int main () 
{ 
	volatile int y = 7; a = &y; 
	foo (); 
	return b > a; 
}
void foo (void) 
{ 
	volatile int x = 5; 
	b = &x; 
}
#endif
//will make in simga platfrom
// mipsel-linux-gcc growing_tack_pointer growing_tack_pointer.c


