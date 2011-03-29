#include <iostream>

using namespace std;
class complex {
	double re, im;
public:
	complex ( double r = 0, double i = 0 ) {
		re = r;
		im = i;
		cout << "constructor" << endl;
	}
	operator double () {
		cout << "operator double " << endl;
		return re;
	}
	double   get_re ()       { 
		return re;
	}
	void     print  () const { 
		cout << "re=" << re << "  im=" << im << endl;
	}
};

template <class T>
T f ( T& x, T& y )
{
cout << "template f" << endl;
return x > y ? x : y;
}

double f ( double x, double y )
{
cout << "ordinary f" << endl;
return x > y ? -x : -y;
}

int main ()
{
	complex a ( 2, 5 ), b ( 2, 7 ), c;
	double x = 3.5, y = 1.1;
	int i, j = 8, k = 10;

	c = f ( a, b );
	cout << "c = ";
	c.print ();
	x = f ( a, y );
	cout << "x = " << x << endl;
	i = f ( j, k );
	cout << "i = " << i << endl;
	cout << "choice is done!" << endl;
	return 0;
} 

