#include <stdio.h>
#include <string.h> //for memset
#include <stdlib.h> //for malloc
#include <unistd.h> //for usleep
#include <pthread.h> //for thread

#include <termio.h>

#define speed 125000
#define uydang 0 //test truong hop dung hoac sai, dung uydang = 0 sai uydang = 1
/*
// vi du minh hoa cach giao tiep bien giua 2 lop
// va minh hoa cach su dung tien trinh trong lop
*/
typedef struct Stest
{
	int per;
	int value;
}var;

void task_second();
int getch( void );

char s[100];
int dem = 0;
int key = 0;

class B
{
public:
	var *varB;
	pthread_t var_thread;
	B();
	~B()
	{
		printf("===> delete B <===\n");
	}
	//de set gia tri tham chieu == cach giao tiep bien giua 2 lop theo dia chi	
	void setVar(var *p); 
	//main thread
	void my_thread();
	//retur 0 if use Ok else for Error
	int fun_use_thread();
	
};

B::B()
{
	varB = (var*)malloc(sizeof(var));
	varB->per 	  = 0;
	varB->value   = 0;
}

void B::setVar(var *p_var_A)
{	
	varB = p_var_A;
}

void *thread(void *ptr_classB)
{
	B *my_class_B = (B*)ptr_classB;
	my_class_B->my_thread();
	return NULL;
}

void B::my_thread()
{
	while(varB->per < 100)
	{
		varB->per ++;		
		printf("\ntime reming: %d\n",(100 - varB->per));
		printf("Nhap chuoi: %s",s);
		usleep(speed);		
		system("clear");
	}		
}

int B::fun_use_thread()
{	
	memset(s,0,sizeof(char));	
	return pthread_create(&var_thread, NULL, thread, this);	
}
//==================================
class A 
{
public:
	var *global;
	B *b;
	A();
	~A()
	{
		delete(b);
		printf("===> delete A <===\n");		
	}
	void test();
};

A::A()
{
	global = (var*)malloc(sizeof(var));
	b = new B();
	b->setVar(global);
}
void A::test()
{
	if (b->fun_use_thread() == 0)
	{
		while(global->per < 100)
		{									
			key = getch();				
			if ( (dem < 100) && (key != '\n'))
			{													
				s[dem] = key;
				s[dem + 1] = '\0';		
				dem ++;				
			}
		}
	}else
	{
		printf("Can not creat thread\n");
	}
}

int getch( void )
{
	char ch;
	int fd = fileno(stdin);	
	struct termio old_tty, new_tty;
	ioctl(fd, TCGETA, &old_tty);
	new_tty = old_tty;
	new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
	ioctl(fd, TCSETA, &new_tty);
	fread(&ch, 1, sizeof(ch), stdin);	
	ioctl(fd, TCSETA, &old_tty);
	return ch;
}

void task_second()
{
#if 0
	char s[100];
	int dem = 0;
	int key = 0;
	system("clear");
	printf("Nhap chuoi: ");
	while (dem < 100)
	{		
		key = getch();	
		if (key == '\n')
			break;
		s[dem] = key;
		s[dem + 1] = '\0';		
		dem ++;
		system("clear");
		printf("Nhap chuoi: %s",s);
	}
	printf("\nChuoi nhap la: %s\n",s);
#endif
}

int main()
{	
	A *a = new A();
	a->test();	
	
	printf("\n");
	delete(a);		
	return 0;
}
//g++ -o main main.cpp -lpthread
