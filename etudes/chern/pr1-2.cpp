/* Написать функцию process, которая принимает два параметра:
 * неизменяемый вектор целых чисел v1;
 * вектор целых чисел v2.
 *
 * Функция проходит по всем элементам векторов v1 и v2. Если i-й элемент вектора v1
 * оказался больше i-о элемента вектора v2, он копируется в конец вектора v2. Добавленные
 * в конец вектора v2 элементы не учитываются при прохождении вектора v2.
 * Затем все элементы вектора v2 выводятся на стандартный поток вывода в прямом порядке.
 * Для доступа к элементам массива и списка использовать только итераторы.*/

#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

void process(vector<int> v1, vector<int> &v2)
{
    vector<int> buf(0);
    vector<int>::reverse_iterator i = v1.rbegin();
    vector<int>::reverse_iterator j = v2.rbegin();
    int def = v1.size() - v2.size();

    if (def > 0) {
        i += def;
    } else {
        j -= def;
    }
    for(; j < v2.rend(); ++i, ++j) {
        if (*i > *j) {
            buf.push_back(*i);
        }
    }

    while (buf.size()) {
        v2.push_back(buf.back());
        buf.pop_back();
    }

    copy(v2.begin(), v2.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    return;
}

int main()
{
    vector<int> a(7,5);
    vector<int> b(6,4);
    process(a, b);
    return 0;
}
