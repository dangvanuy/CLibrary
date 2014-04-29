#include <stdio.h>
#include <string.h> //for memset
#include <stdlib.h> //for malloc
#include <unistd.h> //for usleep
#include <pthread.h> //for thread

#include <termio.h>

void task_second();
int getch( void );

#define speed 1000000
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
	printf("===> set <===\n");
#if !uydang	
	varB = p_var_A; //dung
	//con tro lop B(varB) tro toi vung nho cua con tro lop A(p)
	//va thay doi du lieu tren con tro lop B,
	//phat bieu: lam thay doi gia tri cua con tro lop A
#else
	p_var_A = varB;  //sai
	//con tro lop A tro toi vung nho cua con tro lop B
	//va thay doi du lieu tren con tro lop B
	//phat bieu: khong lam thay doi gia tri cua con tro lop A
	//vi sao: vi pham vi truy cap bien
#endif
}

void *thread(void *ptr_classB)
{
	B *my_class_B = (B*)ptr_classB;
	my_class_B->my_thread();
	return NULL;
}

void B::my_thread()
{
	//printf("===> begin thread <===\n");
	while(varB->per <= 100)
	{
		varB->per ++;
//#if uydang 
		system("clear");
		printf("percen done: %d\n",varB->per);
//#endif
		usleep(speed);
	}	
	//printf("===>  End thread  <===\n");
}

int B::fun_use_thread()
{
	printf("===> fun_use_thread <===\n");
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
	//int  get_per()	  { return global->per; }
	//int  get_value()  {return global->value; }
	//void show_per()	  { printf("global->per: = %d\n",global->per); }
	//void show_value() { printf("global->per: = %d\n",global->value); }
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
#if 0
		while(1)
		{
			if(global->per <= 100)
			{
#if !uydang			
				system("clear");
#endif
				printf("Geting, please wait: %d\%\n",global->per);
			}
			else
			{
				printf("Get done\n");
				break;
			}
			usleep(speed);
		}
#else
	task_second();
#endif
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
#if 1
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
	delete(a);	
	
	return 0;
}
//g++ -o main main.cpp -lpthread
