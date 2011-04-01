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
};

class D: B {
public:
	void f (int a) {
		cout << "f (int)" << endl;
		::a = a;
	}
	void h ();
};

void D::h() {
	f(1.2); 	// IT'S WORK !!!
//	f();  		// error. Not seen f ();
	B:: a = 2; 	
}

int main () {
	D d;
//	f(); 		// error. No f();
	:: f(6);
	return 0;
}
