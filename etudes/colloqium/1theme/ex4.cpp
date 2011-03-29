#include <string.h>

class string {
	char *p;
	int size;
public:
	string ( const char *str ) {
		p = new char [(size = strlen ( str )) + 1];
		strcpy ( p, str );
	}

	~string () { 
		delete [] p; 
	}

	string ( const string &a );

	string& operator= ( const string &a );
};

string:: string ( const string &a ) 
{
	p = new char [ (size = strlen (a.p)) + 1];
	strcpy (p, a.p);

	size = a.size;
}

string & string:: operator= ( const string &a)
{
	p = new char [ (size = strlen (a.p)) + 1];
	strcpy (p, a.p);

	size = a.size;

	return *this;
}

int main (int argc, char * argv[])
{	

		return 0;
}

