/*
Написать функцию, добавляющую в конец списка вещественных чисел элемент, 
значение которого равно среднему арифметическому всех его элементов.
*/

#include <iostream>
#include <list>

using namespace std;

void g (list <double> & l)
{
	list <double> :: iterator  i = l.begin ();	

	double sum = 0;
	int count = 0;

	while ( i != l.end () ) {
		sum += *i;
		++ count;
		++ i;
	}

	l.push_back ( sum / count );
}


int main (int argc, char * argv[])
{	
	list <double> lst;

	lst.push_back (1);
	lst.push_back (2);

	g (lst);

	list <double> :: reverse_iterator i = lst.rbegin ();

	while ( i != lst.rend () ) {
		cout << "*i = " << * i << endl;
		++ i;
	}

	return 0;
}
