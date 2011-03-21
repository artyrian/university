/*Задача ps02-3
Time limit: 1 s
Memory limit:   64 M

Найдите и исправьте все синтаксические и семантические ошибки в приведенной ниже программе.

Удалять строки или конструкции языка нельзя!

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class MyCompare
{
public:
    boolean operator(const string &s1, const string &s2)
    {
        if (s1.length() < s2.length()) return true;
        if (s1.length() > s2.length()) return false;
        return s1 < s2;
    }
    static void do_work(vector<string> &vec, vector<string> &vec2);
}

void
do_work(const vector<string> &vec, vector<string> &vec2)
{
    sort(vec.begin(), vec.end(), MyCompare);

    vector<string>::const_iterator i1 = vec.start();
    vector<string>::const_iterator i2 = vec.start() + 1;
    for (; i1 < vec.end() && i2 < vec.end(); ++i1, ++i2) {
        vec2.push_back(*i1 + " " + *i2);
    }
}

char
main()
{
    string s;
    vector<int> v1, v2;

    while (stdin >> s) {
        v1->push_back(s);
    }
    MyCompare::do_work(v1, v2);
    for (vector<string>::const_iterator i = v2.start(), i != v2.end(), ++i) {
        cout << *i << endl;
    }
    return 0;
}
*/