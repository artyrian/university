#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include "../buffer/buffer.hpp"
#include "tables.hpp"
#include <stdio.h>


class ReadFrom {
	FILE * fp;
public:
	ReadFrom (const char * str);
	int get_symbol ();
	~ReadFrom ();
};


struct Lex {
	type_of_lex	type;
	int 		value;
	int		str_n;

	Lex ();
	Lex (int n);
	Lex (type_of_lex t, int v, int n);

	void print () const; 
};


class LexList {
	struct ListElem {
		Lex		lex;
		ListElem *	next;

		ListElem ();
	};

	ListElem *	first;
	ListElem *	snd;
	ReadFrom 	rf;

	Lex		lex;
	ListElem * create (const Lex & l);
	void add (const Lex & l);
public:
	LexList (const char * path);
	void save ();
	Lex get_lex ();
	void print ();
	~LexList ();
};


class Scanner {
	enum		state 	{ 
					H, 
					NUM, 
					IDENT, 
					KW, 
					ASSIGN, 
					STR, 
					DELIM, 
					NEQ, 
					FN, 
					COMMENT,
					LABEL
				};
	TableLexem	table;
	state		CS;
	int		count_str;
	Buffer * 	buffer;
	int 		digit;
	Lex 		lex; 
	int		save_c;

	int isdelim (int c);
	int look (const char * buf, const char ** list);
public:
	Scanner ();
	Lex feed_symbol (int);
	~Scanner ();
private:
	Lex step (int);

	Lex state_H (int c);
	Lex state_NUM (int c);
	Lex state_IDENT (int c);
	Lex state_KW (int c);
	Lex state_ASSIGN (int c);
	Lex state_STR (int c);
	Lex state_DELIM (int c);
	Lex state_NEQ (int c);
	Lex state_FN (int c);
	Lex state_COMMENT (int c);
	Lex state_LABEL (int c);
};


#endif
