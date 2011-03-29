#include <iostream>

class rational
{
	int p, q;
public:
	rational (int a, int b) {
			p = a;
			q = b;
	}
	rational operator* (const rational & r) const
	{
		return rational (p * r.p, q * r.q);
	}
};



void g ()
{
	rational x (1, 2), y (4, 5), z (1, 1);
	const rational w (1, 8);
	int t = 3;
	
	z = x * y;
	z = w * y;
	z = y * w;

//	z = z * t;
//	z = t * z;
}

int main (int argc, char * argv[])
{	
		g ();

		return 0;
}

