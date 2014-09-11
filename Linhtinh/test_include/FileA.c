#include <stdio.h>

#include "FileA.h"

int g_Var = 10;

void FILEA_Test()
{
	printf("FILE A   : I will include FileB.c in file c and FileB.h in file h of mine\n");
	printf("FILE A   : So, Main can call func in FileB\n");
	printf("FILE A   : Then, i declare a global var in this, for me and FileB using, g_Var = %d\n", g_Var);
}

#include "FileB.c"
