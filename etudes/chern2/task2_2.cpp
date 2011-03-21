#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
using namespace std;
struct Marks
{
    int count;
    int amount;
};
int
main(void)
{
    ifstream from;
    ofstream to;
    map<string, Marks> studs;
    string sur;
    int mrk;
    double average;
    map<string, Marks>::iterator i;
    from.open("02-2.in");
    to.open("02-2.out");
    while(from >> sur >> mrk) {
        studs[sur].count += mrk;
        ++studs[sur].amount;
    }
    for (i = studs.begin(); i != studs.end(); ++i) {
        average = i->second.count;
        average /= i->second.amount;
        to << i->first << " " << average << endl;
    }
    from.close();
    to.close();
    return 0;
}
