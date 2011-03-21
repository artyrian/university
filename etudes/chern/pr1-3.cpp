/* Написать функцию process, принимающую три параметра:
 * вектор целых чисел v1;
 * неизменяемый вектор целых чисел v2;
 * целое число k.

 * Функция должна умножить на k те элементы вектора v1, номера которых записаны в векторе v2.
 * Номера элементов могут повторяться. В этом случае умножать элемент нужно несколько раз. Возможное
 * переполнение игнорируйте. Элементы нумеруются от 1. Если в векторе v2 задан недопустимый номер
 * элемента, он игнорируется. Затем элементы вектора v1 должны быть напечатаны на стандартный поток
 * вывода в прямом порядке. Для доступа к элементам массива и списка использовать только итераторы.
 * Для вывода элементов вектора v1 используйте итератор вывода ostream_iterator.*/

#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

void process(vector<int> &v1, vector<int> v2, int k)
{
    vector<int>::iterator j = v1.begin();

    for(vector<int>::iterator i = v2.begin(); i !=v2.end(); ++i){
        if (*i > 0 && j + *i - 1 < v1.end()){
            *(j + *i - 1) *= k;
        }
    }
    copy(v1.begin(), v1.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    return;
}

int main()
{
    vector<int> a(4,5);
    vector<int> b(0);
    b.push_back(1);
    b.push_back(2);
    b.push_back(1);
    b.push_back(1);
    b.push_back(5);
    process(a, b, 2);
    return 0;
}
