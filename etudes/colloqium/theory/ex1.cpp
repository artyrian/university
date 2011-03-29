#include <stdio.h>

class A {
	char * p;
	int a;
public:
	A () {
		p = new char [10];
	}
	~A () {
		throw 5;
		delete [] p;
	}
};


void f ()
{
	A a;
}

int main (int argc, char * argv[])
{	
	try {
		f ();
		throw 'a';
	}
	catch (int i) {
		printf ("Lol %d", i);
	}
	catch (char c) {
		printf ("ololo %c", c);
	}

	return 0;
}

