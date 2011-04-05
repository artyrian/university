#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include "../lexer/lexer.hpp"

class SymbolException {
	char 	c;
	char * 	str;
	int num_str;
public:
	SymbolException (const char * s, char ch, int n);
	SymbolException (const SymbolException & le);
	~SymbolException ();
	void print () const;
};

class LexException {
	Lex	lex;
	char * 	str;
public:
	LexException (const char * s, const Lex & l);
	LexException (const LexException & le);
	~LexException ();
	void print () const;
};

#endif
