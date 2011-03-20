#include "buffer.hpp"
#include <string.h>

void Buffer:: ext_buffer ()
{
	char *newbuf = new char [size_part * (++part)];
	strcpy (newbuf, buf);
	delete [] buf;
	buf = newbuf;
}

void Buffer:: clear ()
{
	for ( int i = 0; i <= count; i++) {
		buf [i] = '\0';
	}
	count = 0;
	part = 1;
}

void Buffer:: add (int c)
{
	if ( count == (size_part * part - 1) ) {
		ext_buffer ();
	}

	buf [count++] = c;
	buf [count] = '\0';
}

Buffer:: Buffer ()
	: count (0), part (1)
{
	buf = new char [size_part];
	clear ();
}

Buffer:: ~Buffer ()
{
	delete [] buf;
}
