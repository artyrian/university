int x = 0;

void f (int a, int b)
{
	x = a+b;
}

class A {
	int x;
public:
	void f (){x = 2;}
};

class B: public A {
public:
	void f (int a){
		::x = a;
	}
	void g ();
};

void B::g() 
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
	:: f('+', 6);
	return 0;
}
