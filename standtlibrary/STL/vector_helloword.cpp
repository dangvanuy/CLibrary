#include <iostream>
#include <vector>
#include <string>
#include <List>

using namespace std;
void sampleList()
{
	printf("Sample List<>\n");
	List<int> l;
	l.push_back(1);
	l.push_front(1);

	printf("Ll.begin() = %d\n", l.begin());
}
main()
{
   vector<string> SS;
 
   SS.push_back("The number is 10");
   SS.push_back("The number is 20");
   SS.push_back("The number is 30");
 
   cout << "Loop by index:" << endl;
   int ii;
   for(ii=0; ii < SS.size(); ii++)
   {
      cout << SS[ii] << endl;
   }
   
   int a = 100;
   a += 1;
   
   cout << endl << "Constant Iterator:" << endl;
   vector<string>::const_iterator cii;
   for(cii=SS.begin(); cii!=SS.end(); cii++)
   {
      cout << *cii << endl;
   }
   cout << endl << "Reverse Iterator:" << endl;
   vector<string>::reverse_iterator rii;
   for(rii=SS.rbegin(); rii!=SS.rend(); ++rii)
   {
      cout << *rii << endl;
   }
   cout << endl << "Sample Output:" << endl;
   cout << SS.size() << endl;
   cout << SS[2] << endl;
   swap(SS[0], SS[2]);
   cout << SS[2] << endl;


   sampleList();
}

//g++ -o vector_helloword vector_helloword.cpp


