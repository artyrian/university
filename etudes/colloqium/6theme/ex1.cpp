#include <iostream>
/*
y= re=0 im=0
t= 2.8
f(double x, int i)
y= re=6 im=0
y= re=0 im=0
choice is done!

*/
using namespace std;

class complex {
	double re, im;
public:
	complex ( double r = 0, double i = 0) { re = r; im = i; }
	operator         double ()       { return re; }
	double  get_re ()       { return re; }
	void    print  () const { cout << "re=" << re << "  im=" << im << endl; }
};

double f ( double x, int i )
{
	cout << "f(double x, int i)" << endl;
	return x*i;
}

complex f ( complex z, int i )
{
	cout << "f(complex z, int i)" << endl;
	return complex ( z.get_re()*i );
}

int main ()
{
	complex z ( 7, 5 ), y;
	double  x = 1.5, t = 2.8;
	y = f ( z, 2 );
	cout << "y = ";
	y.print();
	t = f ( z, t );
	cout << "t = " << t << endl;
	y = f ( x, 4 );
	cout << " y= ";
	y.print();
	y = f ( z, z );
	cout << "y = ";
	y.print();
	cout << "choice is done!" << endl;

	return 0;
}
