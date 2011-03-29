#include <iostream>

using namespace:: std;
class complex
{
	double   re, im;
public:
	explicit complex ( double r, double i = 0 ) { 
		re = r; im = i; 
	}
	complex () { 
		re = 0; im = 0;
	}
	operator double () { 
		return re; 
	}
	double get_re () { 
		return re; 
	}
	void print () const { 
		cout << "re=" << re << "  im=" << im << endl;
	}
};

double f ( complex z, int i )
{
cout << "f(complex z, int i)" << endl;
return z.get_re() * i;
}

int f ( complex x, complex y )
{
cout << "f(complex x, complex y)" << endl;
return x.get_re() * y.get_re();
}

complex f ( complex x, double t )
{
cout << "f(complex x, double t)" << endl;
return complex ( x.get_re() * t );
}

int main ()
{
complex a ( 2, 3 ), b ( 4, 7 );
double  x = 1.5;
int i = 15;

x = f ( a, x );
cout << "x = " << x << endl;
//x = f ( x, a );
cout << "x = " << x << endl;
//b = f ( complex (i), a );
cout << "b = ";
b.print ();
x = f ( b, 'a' );
cout << "x = " << x << endl;

cout << "choice is done!" << endl;
return 0;
}
