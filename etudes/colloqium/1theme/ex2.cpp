#include <stdio.h>

class A {
	int p1;
	int p2;
public:
	A (int p1 = 0, int p2 = 0 ) {};
};

int main (int argc, char * argv[])
{	
	A a;
	A b (1);
	A c (1, 2);
	A d ('1', 1);

	return 0;
}

