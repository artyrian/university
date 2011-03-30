/*
Напишите функцию process, которая принимает три параметра: 
неизменяемый вектор целых чисел;
список целых чисел;
целый положительный параметр: шаг по вектору.

Функция проходит элементы вектора целых чисел с заданным шагом и копирует их в соответствующие элементы списка 
(список проходится с шагом 1). 
Затем элементы списка выводятся на стандартный поток вывода в обратном порядке. 
Для доступа к элементам массива и списка использовать только итераторы. 
*/

#include <iostream>
#include <list>
#include <vector>


using namespace std;


void process (const vector <int> v, list <int> l, unsigned int k)
{
	vector <int> :: const_iterator i = v.begin ();
	list <int> :: iterator j = l.begin ();

	while ( (j != l.end ()) && (i < v.end ()) ) {
		*j = *i;
		i += k;
		++ j;
	}
	
	for (	i = v.begin (); i < v.end (); ++ i ) { 
		cout << *i << endl;
	}

}



int main (int argc, char * argv[])
{	
	vector <int> vect (2,5);
	list <int> lst (3, 4);
	int k = 2;

	process (vect, lst, k);

	return 0;
}
