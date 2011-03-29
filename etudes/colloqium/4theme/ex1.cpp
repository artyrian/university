#include <iostream>

using namespace:: std;

class A {
public:
	void f1 () {
		cout  << "f1_A" << endl;
	}

	virtual void f1 ( const int c )  { 
		cout  << "f1_A_int" << endl;
	}
	virtual void f2 () { 
		cout  << "f2_A" << endl; 
	}

	virtual int  f3 ( const int k ) { 
		cout  << "f3_A" << endl; return k + 10;
	}
};

class D: public A {
public:
	void f1 ()               { cout << "f1_D" << endl; }
	virtual void f1 ( const char x ) { cout << "f1_D_char" << endl; }
	void f2 ()               { cout << "f2_D" << endl; }
//	virtual void f3 ( const int a )  { cout << "f3_D" << endl; }
};

void g ()
{
	A x;
	D y;
	A *p = &x;

	p->f1 ();
	p->f1 ( '1' );
	p->f1 ( 1 );
	p->f2 ();
//	p->f3 ();

	p = &y;
	p->f1 ();	// error
	p->f1 ( '1' );
	p->f1 ( 1 );
	p->f2 ();
//	p->f3 ();
}

int main (int argc, char * argv[])
{	
	g ();

	return 0;
}

