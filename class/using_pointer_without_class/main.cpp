#include <stdio.h>
#include <string.h>
#include <unistd.h>


typedef struct SStructInStruct
{
	int isFinishTaskB;
}SStructInStruct;

typedef struct Status{
	int FinishTaskA;
	SStructInStruct m_structB;
}SStatus;

void SetGlobalStatus(SStatus *g_Status);
void SetGlobalSStructInStruct(SStructInStruct *g_Status);

class CTest
{
private:
	SStatus *m_Status;
public:
	CTest(){
		m_Status = new SStatus;
		//m_Status->FinishTaskA = 0;
		
		//SetGlobalStatus(m_Status); ok
		memset(&m_Status->m_structB, 0, sizeof(SStructInStruct));
		SetGlobalSStructInStruct(&m_Status->m_structB);
	}
	
	void GetisFinishTaskB()
	{
		printf("in class test isFinishTaskB = %d\n",  m_Status->m_structB.isFinishTaskB);
	}
};

SStatus *p_Status;
SStructInStruct *p_StatusA;

void SetGlobalStatus(SStatus *g_Status)
{
	p_Status = g_Status;
	printf("p_Status->FinishTaskA = %d\n", p_Status->FinishTaskA);
}
void SetGlobalSStructInStruct(SStructInStruct *g_Status)
{
	p_StatusA = g_Status;
	printf("p_Status->isFinishTaskB = %d\n", p_StatusA->isFinishTaskB);
}

int main()
{
	CTest *p_test = new CTest();
	
	//printf("Main p_Status->FinishTaskA = %d\n", p_Status->FinishTaskA);
	
	printf("Main p_Status->isFinishTaskB = %d\n", p_StatusA->isFinishTaskB);
	p_StatusA->isFinishTaskB = 10;
	p_test->GetisFinishTaskB();
	return 0;
}
