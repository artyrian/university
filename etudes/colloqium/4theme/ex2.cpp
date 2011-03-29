#include <iostream>

using namespace std;

class B {
public:
	void f () { cout << "B::f\n"; }
	virtual void g () { cout << "B::g\n"; }
	void h () { cout << "B::h\n"; }
};
 
class D : public B {
public:
	void f () { cout << "D::f\n"; }
	void g () { cout << "D::g\n"; }
	virtual void h () { cout << "D::h\n"; }
};
 
void P ( B * pb,   D * pd )
{
	pb->f ();
	pb->g ();
	pb->h ();
	pd->f ();
	pd->g ();
	pd->h ();
}
 
int main ()
{
  P ( new B, new D );
  P ( new D, new D );

  return 0;
}
