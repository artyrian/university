#ifndef _LEXLIST_HPP_
#define _LEXLIST_HPP_

#include "lexer.hpp"
#include <stdio.h>


class ReadFrom {
	FILE * fp;
public:
	ReadFrom (const char * str);
	int get_symbol ();
	~ReadFrom ();
};



class LexList {
	struct ListElem {
		Lex		lex;
		ListElem *	next;

		ListElem ();
	};

	ListElem *	first;
	ReadFrom 	rf;

	ListElem * create (const Lex & l);
	void add (const Lex & l);
public:
	LexList (const char * path);
	void save ();
	Lex get_lex () const;
	void print ();
	~LexList ();
};


#endif
