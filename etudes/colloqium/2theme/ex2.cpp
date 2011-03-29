class rational {
	int p, q;
public:
	rational ( int a, int b )
	{
		p = a;
		q = b;
	}
	// ...

	rational operator++ (int) {
		rational tmp (*this);
		++p;
		++q;
		return tmp;
	}

	rational operator++ () {
		++p;
		++q;
		return (* this); 
	}
};

void g ()
{
	rational x ( 1, 2 ), z ( 1, 1 );
	const rational y ( 1, 8 );
	z = x++;
//	z = y++;
	z = ++x;
//	z = ++y;

	x = x++ ++;
	x = ++ ++x;
}

int main (int argc, char * argv[])
{	
	g ();

	return 0;
}

