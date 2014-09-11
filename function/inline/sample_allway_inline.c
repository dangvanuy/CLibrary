#include <stdio.h>


//GCC does not inline any functions when not optimizing 
//unless you specify the `always_inline' attribute for the function, like this:
inline void foo (const char) __attribute__((always_inline));

struct _A
{
	int a;
};

#define DECLARE_INTERFACE( IFACE )                \
     typedef struct _##IFACE IFACE;

DECLARE_INTERFACE(A)

inline void foo(const char c)
{
	printf("Char %c\n", c);
}

int main()
{
	A ma;
	ma.a = 100;
	char i;
	for(i = 65; i < 65 + 26; i ++){
		foo(i);
	}
	return 0;
}
