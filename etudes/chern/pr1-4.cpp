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

void
process(vector<int> v, list<int> &lst)
{
    vector<int>::iterator i = v.begin();
    list<int>::iterator j = lst.begin();
    int n = 1;

    sort(v.begin(),v.end());
    for (i = v.begin(); i != v.end() && *i < 1; ++i) {
        //тут пусто
    }
    for ( ; i != v.end(); ++i) {
        if ((i != v.begin() && i[-1] != *i) || i == v.begin()) {
            for ( ; j != lst.end() && n != *i; ++j, ++n) {
                //тут тоже пусто
            }
            if (j == lst.end()) {
                return;
            } else {
                lst.erase(j);
                ++n;
                --j;
                ++j;
            }
        }
    }
    return;
}

int main()
{
    vector<int> a(0);
    a.push_back(1);
    a.push_back(1);
    a.push_back(3);
    a.push_back(2);
    list<int> b(1,1);
    b.push_back(44);
    b.push_back(3333);
    b.push_back(123);

    process(a, b);
    copy(b.begin(), b.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    return 0;
}
