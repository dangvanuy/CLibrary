#include <stdio.h>

#include <sched.h> //struct sched_param 

#define MAX_PSI 3

typedef char uint8_t;
typedef long uint32_t;
typedef long int off_t;

typedef struct tPsiGroup
{
   int		used;
} tPsiGroup;

static tPsiGroup gPsiGroups[MAX_PSI];

//define struct pointer
typedef struct Psi* tPsi;

int OpenSA(tPsi *pPsi, int index)
{
	if (pPsi == NULL){
		printf("Parameter is ERROR\n");
		return 1;
	}
	tPsiGroup* pGroup = NULL;
	gPsiGroups->used 	= 1;
	pGroup = &gPsiGroups[index];
	
	*pPsi = (tPsi)pGroup;
	printf("Setup using pointer Struct done for index: %d\n", index);
	return 0;
}


int getIndex(tPsi psi)
{
	tPsiGroup* pPriv = NULL;
	uint8_t*   ptr = (uint8_t*)psi;
	uint8_t*   array = (uint8_t*)gPsiGroups;
	uint32_t   idx = 0;
	size_t     size = sizeof( gPsiGroups );
	off_t      offset = 0;

	/* Make sure that the given pointer is in the range of this ciphers array */
	if(( ptr >= array ) && ( ptr < ( array + size )))
	{
		offset = ptr - array;

		/* Make sure that the offset from the array is an even multiple of the
		structure size */
		if( 0 == ( offset % sizeof( *pPriv )))
		{
			idx = offset / sizeof( *pPriv );
			if(( idx < MAX_PSI ) && ( &gPsiGroups[ idx ] == (tPsiGroup*)psi ))
			{
				pPriv = &gPsiGroups[ idx ];
				printf("getIndex::gPsiGroups[%d].used = %d\n", idx, gPsiGroups[idx].used);
			}
		}
   }

   //return pPriv;
   printf("Index is: %d\n", idx);
   return idx;
}

void printf_binary( const char *st, unsigned int num){
	unsigned int n = num;
	printf("%s = ", st);
	char stack[100] = {0, };
	int index = 0;
	while (n) {
		if (n & 1){
			//printf("1");
			stack[index++] = '1';
		} else{
			//printf("0");
			stack[index++] = '0';
		}		
		n >>= 1;
	}
	for (int i = index -1; i >= 0; i--){
		printf("%c", stack[i]);
	}
	printf("b, 0x%x(%dd)\n", num, num);
}

void TestData()
{
	unsigned long RMuint320_a = 0xfffccfb;
	printf("sizeof(RMuint320_a)= %d\n", sizeof(RMuint320_a));
	printf_binary("RMuint320_a", RMuint320_a);
	printf_binary("RMuint320_a = RMuint320_a & 0xff", (RMuint320_a = RMuint320_a & 0xff));
	printf_binary("RMuint320_a = RMuint320_a << 8", RMuint320_a = RMuint320_a << 8);
	printf_binary("RMuint320_a | 0xab", RMuint320_a | 0xab);
	
	RMuint320_a = 0xafffcfb;
	printf_binary("RMuint320_a = RMuint320_a >> 8", RMuint320_a = RMuint320_a >> 8);
	printf_binary("RMuint320_a = RMuint320_a & 0xff", RMuint320_a = RMuint320_a & 0xff);
	
	//RMuint320_a = 0xafbfcfd;
	RMuint320_a = 0xf1;
	//1111     	= 1000.0000 0000.0000
	//1111.0001	= 1000.0000 0000.0000 00
	printf_binary("RMuint320_a", RMuint320_a);
	printf_binary("RMuint320_a = 1 << RMuint320_a", RMuint320_a = 1 << RMuint320_a);
}

void test_sched_param()
{
	struct sched_param param = {0,};
	param.sched_priority = 1;
	printf("param.sched_priority = %d\n", param.sched_priority);
}

int main()
{
	tPsi mpsi;
	OpenSA(&mpsi, 2);
	
	//How to use var used in struct tPsiGroup
	//printf("use = %d\n", mpsi->used); //Error, can not use like this, request for member "used" in "mpsi", 
										//which is of non-class type "Psi*" 
	tPsiGroup *pGroup = NULL;
	pGroup = (tPsiGroup*)mpsi;	
	printf("pGroup->use = %d\n", pGroup->used); //only convert to tPsiGroup must use be var pGroup->use
												//but value is wrong
	
	//How to find index of mpsi in gPsiGroups[]
	getIndex(mpsi); //output is 2
	
	OpenSA(&mpsi, 0);
	getIndex(mpsi); //output is 0
	
	OpenSA(&mpsi, 1);
	getIndex(mpsi);
	
	
	printf("=========== Test pointer ===============\n");
	
	
	TestData();
	
	test_sched_param();
	return 0;
}


//g++ -o pointer_var  pointer_var.cpp


