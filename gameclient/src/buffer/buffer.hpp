#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

class Buffer {
	static const int size_part = 32;
	char * buf;
	int count;
	int part;

	void ext_buffer ();
public:
	Buffer ();
	void add (int c);
	void clear ();
	char * get ();
	~Buffer ();
};

#endif
