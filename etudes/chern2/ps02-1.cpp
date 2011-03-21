/* Задача ps02-1
 * Time limit:	1 s
 * Memory limit:	64 M

 * Напишите функцию process, которая принимает по ссылке три параметра:

    * вектор целых (int) чисел;
    * неизменяемый список целых (int) чисел;
    * целый положительный параметр: шаг по вектору.

 * Функция проходит элементы вектора целых чисел с заданным шагом, а список с шагом 1. Если очередной 
 * элемент списка меньше нуля, он копируется в соответствующий элемент вектора.

 * Затем элементы вектора выводятся на стандартный поток вывода в прямом порядке.

 * Функция должна возвращать количество скопированных элементов списка.

 * Для доступа к элементам массива и списка использовать только итераторы. */
#include <iostream>
#include <list>
#include <vector>
#include <iterator>

using namespace std;

int 
process(vector<int> &v, const list<int> &l, const unsigned int &step)
{
    int n = 0;
    vector<int>::iterator i = v.begin();
    for (list<int>::const_iterator j = l.begin(); j != l.end(); ++j){
        if (i >= v.end()){
            break;
        }
        if (*j < 0){
            *i = *j;
            ++n;
        }
        i += step;
    }
    
    copy(v.begin(), v.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    return n;
}



int main()
{
    vector<int> v(4,5);
    list<int> l(10, -2);
    process(v, l, 2);
    return 0;
}
