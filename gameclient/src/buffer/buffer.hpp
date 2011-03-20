#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

class Buffer {
	const int buf_size = 32;
	char * str;
	int count;
	int part;

	void ext_buffer ();
	void clear ();
public:
	Buffer ();
	void add ();
	~Buffer ();
};

#endif
