#include <iostream>
#include <list>

using namespace std;

void f (list <int> l)
{
	list <int> :: iterator k = l.begin ();
	while ( k != l.end () ) {
		k = l.insert (k, *k);
		++ k;
		++ k;
	}

	list <int> :: reverse_iterator i = l.rbegin ();
	while ( i != l.rend () ) {
		cout << *i << endl;	
		++ i;
	}
}

int main (int argc, char * argv[])
{	
	list <int> lst (2, 1);

	f (lst);
	
	


	return 0;
}

