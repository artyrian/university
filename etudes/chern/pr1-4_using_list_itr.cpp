/* Написать функцию process, принимающую два параметра:
 * неизменяемый вектор целых чисел v;
 * список целых чисел lst.

 * Функция должна удалить из списка lst элементы с номерами, заданными в векторе v. Элементы
 * списка нумеруются от 1. Номера элементов списка отражают позиции элементов на момент начала
 * работы программы. Если номер повторяется в массиве более одного раза, все вхождения, кроме
 * первого, игнорируются. Если число в массиве не является допустимым номером элемента в
 * списке, оно игнорируется. Для доступа к элементам массива и списка использовать только
 * итераторы.*/

#include <iostream>
#include <vector>
#include <iterator>
#include <list>
#include <algorithm>
using namespace std;

void process(vector<int> v, list<int> &lst)
{
    vector<int>::iterator i = v.begin();
    int n = 1;
    for(list<int>::iterator j = lst.begin(); j != lst.end(); ++n){
        for(i = v.begin(); i != v.end() && n != *i; ++i)
            ;
        if (n == *i){
            lst.erase(j);
            --j;
        }
        ++j;
    }
    return;
}

int main()
{
    vector<int> a(0);
    a.push_back(3);
    a.push_back(4);
    list<int> b(1,1);
    b.push_back(44);

    b.push_back(3333);
    process(a, b);
    copy(b.begin(), b.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    return 0;
}
