#include <iostream>
#include <list>

using namespace std;

void g (list <double> &lst)
{
	list <double> :: const_iterator p = lst.begin ();
	double s (0);
	int n;
	while ( p != lst.end () ) {
		s = s + *p;
		++n;
		++p;
	}
	if ( n != 0 ) {
		lst.push_back ( s/n);
	}
}

int main (int argc, char * argv[])
{	
	list <double> l;
	l.push_back (1);
	g (l);	

	return 0;
}

