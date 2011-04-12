#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include "../lexer/lexer.hpp"
#include "../lexer/rpn.hpp"

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

//----------------------------------------------------------

class LexException {
	Lex	lex;
	char * 	str;
public:
	LexException (const char * s, const Lex & l);
	LexException (const LexException & le);
	~LexException ();
	void print () const;
};

//----------------------------------------------------------
class PolizException {

};


class PolizExceptionNotInt : public PolizException {

public:
	PolizExceptionNotInt (PolizElem * op);
};


class PolizExceptionNotLabel : public PolizException {

public:
	PolizExceptionNotLabel (PolizElem * op);
};


class PolizExceptionNotNum : public PolizException {

};

//----------------------------------------------------------

#endif
