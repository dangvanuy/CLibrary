#include <stdio.h>
#include "FileA.h"

int main()
{
	printf("Main     : I will test include file c\n");
	printf("Main     : I will include FileA.h\n");
	printf("Main     : Then I call a func in FileA.h\n");
	FILEA_Test();
	printf("Main     : Yes FileA, I will try call a func in FileB\n");
	FILEB_Test();
	printf("Main     : Verry good, thanks FileA and FileB\n");
	return 0;
}

