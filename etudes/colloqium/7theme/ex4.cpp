#include <iostream>
#include <string.h>

using namespace std;

template <>
const char * max <const char *> (const char * x, const char * y)
{
	int i = 0;

	while (x[i] != '\0' || y[i] != '\0' ) {
		if ( x [i] != y [i]) 
			return x [i] > y [i] ? x : y;
		else
			++i;
	}

	return x[i] != '\0' ? x : y;
}

template <class T>
T max (T& x, T& y)
{
	  return x > y ? x : y;
}
 
int main ()
{
	double x = 1.5, y = 2.8, z;
	int i = 5, j = 12, k;
	char *s1; 
	char *s2, *s3;

	s1 = new char [ strlen ("abft") + 1];
	strcpy (s1, "abft");
	s2 = new char [ strlen ("abxde") + 1];
	strcpy (s2, "abxde");
	

	z = max ( x, y );
	cout << "z = "<< z << endl;
	k = max <int>(i, j);
	cout << "k = "<< k << endl;
	//z = max (x, i);
	cout << "z = "<< z << endl;
	z = max <double> ( y, j );
	cout << "z = "<< z << endl;
	s3 = max  (s1, s2);
	cout << "s3 = "<< s3 << endl;
	cout << "choice is done!" << endl;
	return 0;
}
