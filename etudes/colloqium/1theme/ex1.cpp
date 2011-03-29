#include <stdio.h>
#include <iostream>

using namespace:: std;

class A
{
	int a, b;
public:
	A ( const A &x )  { a = x.a; b = x.b; cout << 1; }
	A ( float a = 0, int b = 0 ) { this->a = a; b = a; cout << 2; }
};
 
void f ()
{
	A x ( 1 );
	A y;
	A z = A ( 2, 4 );
	A t ( 3.5, 2 );
	A s = 6;
	A w = z;
	t = z = w;
}

int main (int argc, char * argv[])
{	
		f ();

		return 0;
}

