#include <iostream>

using namespace std;

class M {
public:
  int m;
};
 
class N {
public:
  int n;
  void g ();
};

/* 
comment
 */
 
struct A: M, virtual N {
  char g ( char );
};
 
struct B: M, virtual N {
  void g ( int );
};
 
class D: public A, public B {
public:
  void f ();
};
 
void D::f ()
{
  char c = 'a';
  A::m = 1;
  n = 1;
  c = A::g ( c );
  B::g ( 2 );
  A::N::g ();
}

int main (int argc, char * argv[])
{	
	D d;
	d.f ();

	return 0;
}

