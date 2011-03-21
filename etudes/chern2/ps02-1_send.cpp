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
    for (list<int>::const_iterator j = l.begin(); j != l.end(); ++j) {
        if (i >= v.end()) {
            break;
        }
        if (*j < 0) {
            *i = *j;
            ++n;
        }
        i += step;
    }
    
    copy(v.begin(), v.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    return n;
}
