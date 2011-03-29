int x = 0;

void f (int a, int b)
{
	x = a+b;
}

class A {
	int x;
public:
	void f () { x = 2; }
};

class B: public A {
public:
	void f (int a) {
		::x = a;
	}

	void g ();
};

void B::g() {
	A::f (); 
	f (1);
	::f (5 , 1);
	::x = 2; 
}

int main () {
	B b;
//	f(5);
	f ('+', 6);
	return 0;
}
