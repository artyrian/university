#include <vector>
#include <iostream>

using namespace std;
 
int main () {
	vector < int > V ( 100,5 );
	vector < int > :: const_iterator  p = V.begin ();
	while (p != V.end ()) {
		cout << *p << ' ';
		++p;
	}
	cout << endl;
}
