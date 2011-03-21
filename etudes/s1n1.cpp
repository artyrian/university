#include <stdio.h>

class Complex
{
	double re, im;
public:
	Complex (int r, int i)
	{
		re = r;
		im = i;
	}
	Complex (int r)
	{
		re = r;
		im = 0;
	}
	Complex ()
	{
		re = 0;
		im = 0;
	}
};

int main ()
{
	Complex z; 

	return 0;
}
