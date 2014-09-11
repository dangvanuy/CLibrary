#include <stdio.h>

template<typename Value>
class HashSet
{
public:
	HashSet()
	{
		printf("Constructor\n");
	}
	~HashSet()
	{
		printf("Destructor\n");
	}
};

int main()
{
	HashSet<int> *myHash = new HashSet<int>();
	printf("Hello hashset\n");
	return 0;
}

