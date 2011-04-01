#include <iostream>

using namespace std;

int x = 0;

void f (int a, int b)
{
	x = a+b;
}


class A {
	int x;
public:
	A () { cout << "c. A" << endl; }
	A (const A & a) {
		cout << "c.c. A" << endl;
	}
	void f (){x = 2;}
	~A () {
		cout << "d. A" << endl;
	}
};

class B: public A {
public:
	B () { cout << "c. B" << endl; }
	B (const B & b) {
		cout << "c.c. B" << endl;
	}
	void f (int a){
		::x = a;
	}
	
	void g ();

	~B () {
		cout << "d. B" << endl;
	}

};



B ret (B & x, B & y) 
{
	return x;
}

void l ()
{
	B b;			// конструктор A, конструктор B;
	b = ret (b, b);		// к.к. А, к.к B
				// деструктор B, деструктор A;
}

void B:: g() 
{
//	f();		//  Перекрытие видимости.
	B:: f(1);
//	f(5 , 1);	// Перекрытие видимости.
//	x = 2;		// в private лежит.
}

int main () 
{
	B b;
//	f(5);		//должно быть два параметра.

	cout << "Start test:" << endl;
	l ();
	cout << "End test:" << endl;

	:: f('+', 6);
	return 0;
}
