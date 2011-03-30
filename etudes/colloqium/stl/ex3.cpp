/*
Написать функцию, 
формирующую по заданному списку целых чисел 
вектор из элементов списка с четными значениями 
и распечатывающую его.
*/

#include <iostream>
#include <list>
#include <vector>

using namespace std;

void g (list <int> & l, vector <int> & v)
{
	list <int> :: const_iterator i = l.begin ();

	while ( i != l.end () ) {
		if ( ! (*i % 2 ) ) {
			v.push_back (*i);
		}
		++ i;
	}

	for (int j = 0; j < v.size (); j++) {
		cout << v [j] << endl;
	}
}

int main (int argc, char * argv[])
{	
	list <int> lst;
	vector <int> vect;

	lst.push_back (1);
	lst.push_back (2);
	lst.push_back (3);
	lst.push_back (4);


	g (lst, vect);

	return 0;
}

