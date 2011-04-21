#include <iostream>

using namespace std;

double a = 0;

void f (double x)
{
	cout << "f (double)" << endl;
	a = x;
}

struct B {
	double a;
	void f () {
		a = 2;
	}

	B () {
		cout << "c. B" << endl;
	}

	B (const B & b) {
		cout << "c.c B" << endl;
	}
	
	B operator-- (int) {
		B tmp = *this;
		-- a;
		return tmp;
	}
	~B () {
		cout << "d. B" << endl;
	}
};

class D: B {
public:
	D () {
		cout << "c. D" << endl;
	}
	D (const D & d) {
		cout << "c. c. D" << endl;
	}
	void f (int a) {
		cout << "f (int)" << endl;
		::a = a;
	}

	void h ();
	~D () {
		cout << "d. D" << endl;
	}
};

void empty (D & a, D b) {

}

void D::h() {
	f(1.2); 	// IT'S WORK !!!
//	f();  		// error. Not seen f ();
	B:: a = 2; 	
}

int main () {
	cout << "start" << endl;
	D d;	// c B	c D
	empty (d, d);	// c.c B!!! (right c.B) 	c.c D	d. D	d.B
	cout << "end" << endl;	
					//	d. D	d. B
	return 0;
}


