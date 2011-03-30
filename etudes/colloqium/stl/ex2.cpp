/*
Написать функцию, 
формирующую по заданному вектору целых чисел список 
из элементов вектора с четными значениями 
и распечатывающую его.
*/

#include <iostream>
#include <vector>
#include <list>

using namespace std;

void g (vector <int> & v)
{
	list <int> l;

	vector <int> :: iterator i = v.begin ();
	list <int> :: iterator j = l.begin ();

	while ( i != v.end () ) {
		l.push_back ( *(i++) );	
	}

	j = l.begin ();
	cout << "print list:" << endl;

	while ( j != l.end () ) {
		cout << *(j++) << endl;
	}
}


int main (int argc, char * argv[])
{	
	vector <int> vect;

	vect.push_back ( 10 );
	vect.push_back ( 20 );
	vect.push_back ( 30 );
	vect.push_back ( 40 );
	vect.push_back ( 17 );
	vect.push_back ( 50 );
	
	g (vect);	

	return 0;
}
