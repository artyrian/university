/* Напишите функцию process, которая принимает три параметра:
 * неизменяемый вектор целых чисел;
 * список целых чисел;
 * целый положительный параметр: шаг по вектору.

 * Функция проходит элементы вектора целых чисел с заданным шагом и копирует их
 * в соответствующие элементы списка (список проходится с шагом 1).
 * Затем элементы списка выводятся на стандартный поток вывода в обратном порядке.
 * Для доступа к элементам массива и списка использовать только итераторы. */

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
using namespace std;

void
process(vector<int> const v, list<int> &lst, unsigned int n)
{
    list<int>::iterator k = lst.begin();

    for (vector<int>::const_iterator i = v.begin(); i < v.end() && k != lst.end(); i += n, ++k){
            *k = *i;
    }
    copy(lst.rbegin(), lst.rend(), ostream_iterator<int>(cout, " "));
    return;
}
int main ()
{
    list<int> l(10,1);
    vector<int> v(4,5);
    process(v, l, 3);
    return 0;
}
