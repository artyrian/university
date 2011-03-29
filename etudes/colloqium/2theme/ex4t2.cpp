#include <string>
#include <string.h>

class string {
	char *p;
	int   size;
public:
	string ( const char *str ) {
		p = new char [(size = strlen(str)) + 1];
		strcpy ( p, str );
	}

	string ( const string &a ) {
		p = new char [(size = a.size) + 1];
		strcpy ( p, a.p );
	}

	~string () { 
		delete[] p; 
	}

	string& operator= ( const string &a ) {
		if ( this == &a ) 
			return *this;
		delete [] p;
		p = new char [(size = a.size) + 1];
		strcpy ( p, a.p );
		return *this;
	}

	string operator* ( const int n ) const;
};





string string:: operator* (const int n) const
{
	char * tmp_p = new char [ n * strlen (p) + 1 ];	

	for (int k = 0; k < n; k++) {
		int i = 0;
		while (p [i] != '\0') {
			tmp_p [n * size + i] = p [i];
			++i;
		}
		tmp_p [i] = '\0';
	}

	return string (tmp_p);
}


void g ()
{
  string s1 ( "11111" );
  string s2 = s1;
  const string s3 ( "222222222" );
  int t = 5;
 
  // ...
  s1 = s2 * t;
//  s1 = t * s2;
  s1 = s3 * t;
//  s1 = t * s3;
  s1 = s2 * 3;
}

int main (int argc, char * argv[])
{	

		return 0;
}

