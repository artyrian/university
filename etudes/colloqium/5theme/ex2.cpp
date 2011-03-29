#include <iostream>

using namespace std;

class V {
public:
  int v;
  void x ( int );
};
 
class A {
public:
  int a;
};
 
struct B: A, virtual V {
  double x;
};
 
struct C: A, virtual V {
  char x;
};
 
class D: public B, public C {
public:
  void f ();
};
 
void D::f ()
{
	v = 1;
	B:: a = 1;
	B::V:: x ( 6 );
 	B:: x = 1.2;
	C:: x = 'a';
}

int main (int argc, char * argv[])
{	
	D d;
	d.f ();

	return 0;
}

