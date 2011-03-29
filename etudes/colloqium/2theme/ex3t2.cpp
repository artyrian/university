#include <iostream>
#include <string.h>
#include <string>

class string {
	char *p;
	int   size;
public:
	string ( const char *str) {
		p = new char [(size = strlen(str)) + 1];
		strcpy( p, str );
	}

	string ( const string &a ) {
		p = new char [(size = a.size) + 1];
		strcpy ( p, a.p );
	}

	~string () {
		delete[] p;
	}

	string& operator= ( const string &a ) {
		if ( this == &a ) {
			return (* this);
		}

		delete [] p;
		p = new char [(size = a.size) + 1];
		strcpy ( p, a.p );

		return (* this);
	}

	string operator+ (const string &a) const {
		char * tmp_p = new char [ strlen(p) + strlen(a.p) + 1 ];
		return string (tmp_p);
	}
};

void g ()
{
	string s1 ("11111");
	string s2 = s1;
	string s3 ( "222222222" );
	const string s4 ( "4444444" );
	// ...
	s1 = s2 + s3;
	s1 = s3 + s4;
	s1 = s4 + s3;
	s1 = s3 + "abcd";
}

int main (int argc, char * argv[])
{	
	g ();

	return 0;
}

