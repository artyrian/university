void Buffer::ext_buffer ()
{
	char *newbuf = new char [buf_size * (++part)];
	strcpy (newbuf, buf);
	delete [] buf;
	buf = newbuf;
}

void clear ()
{
	for ( int i = 0; i <= count; i++) {
		buf [i] = '\0';
	}
	count = 0;
	part = 1;
}

char * Buffer::add (char c)
{
	if ( count == (buf_size * part - 1) ) {
		ext_buffer ();
	}

	buf [count++] = c;
	buf [count] = '\0';

	return buf;
}

Buffer::Buffer ()
	: count = 0, part = 1;
{
	buf = new char [buf_size];
	clear ();
}

Buffer::~Buffer ()
{
	delete [] buf;
}
