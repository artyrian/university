#include <stdio.h>



class Rational
{
	int nm, dnm;
	
	

public:
	Rational(int tnm, int tdnm)
	{
		nm = tnm, dnm = tdnm;
		Normalize();
	}
	Rational(int tnm)
	{
		nm = tnm, dnm = 1;
	}
	Rational()
	{
		nm = 0; dnm = 1;
	}
	Rational operator+ (const Rational & op2) const
	{
		return Rational(nm * op2.dnm + op2.nm * dnm, dnm * op2.dnm);
	}
	Rational operator- (const Rational & op2) const
	{
		return Rational(nm * op2.dnm - op2.nm * dnm, dnm * op2.dnm);
	}
	Rational operator* (const Rational & op2) const
	{
		return Rational(nm * op2.nm, dnm * op2.dnm);
	}
	Rational operator/ (const Rational & op2) const
	{
		return Rational(nm * op2.dnm, dnm * op2.nm);
	}
	void print()
	{
		printf ("This is [%d/%d]\n", nm, dnm);
	}
private:
	void Normalize();
};

void Rational::Normalize()
{
	int tnm, tdnm;

	if ( nm < 0 )
		tnm = - nm;
	else
		tnm = nm;

	tdnm = dnm;

	while ( tnm != 0 )
	{
		int tmp;
		tmp = tdnm % tnm;
		tdnm = tnm;
		tnm = tmp;
	}

	nm /= tdnm;
	dnm /= tdnm;
}

int main(int argc, char * argv[])
{	
	Rational r, r1(1, 2), r2(2, 3);	
	
	r.print();
	r1.print();
	r2.print();
	r = r1 * r2;
	r.print();

	return 0;
}

