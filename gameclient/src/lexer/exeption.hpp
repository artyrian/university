#ifndef _EXEPTION_HPP_
#define _EXEPTION_HPP_

#include "lexer.hpp"

class LexExeption {
	Lex	lex;
	char * 	str;
	int 	num_str;
public:
	LexExeption (const char * s, const Lex & l, int n);
	LexExeption (const LexExeption & le);
	~LexExeption ();
	void print () const;
};

#endif
