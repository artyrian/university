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
	int		strnum;
	type_of_lex	type;
	int 		value;

	Lex (int = 0, type_of_lex = LEX_NULL, int = 0);

	void print () const; 
};




class LexList {
	struct ListElem {
		Lex		lex;
		ListElem *	next;
	};
	ListElem *	first;
	ListElem *	snd;


	ListElem * create_elem (const Lex & l);
	void put_lex (const Lex & l);
public:
	LexList ();
	~LexList ();

	void fill (const char * path);

	Lex get_lex_from_list ();

	void print ();
};




class Scanner {
	enum	state 	{ 
		H, 
		NUM, 
		IDENT, 
		KW, 
		ASSIGN, 
		STR, 
		DELIM, 
		NEG, 
		FN, 
		COMMENT,
		LABEL
	};
	TableLexem *	table;
	state		CS;
	int		count_str;
	Buffer * 	buffer;
	int 		digit;
	int		save_c;

	Lex		save_lex;

	int isdelim (int c);
	int look (const char * buf, const char ** list);
public:
	Scanner ();
	bool feed_symbol (int);
	~Scanner ();
	Lex get_save_lex ();
private:
	bool step (int);

	bool state_H (int c);
	bool state_NUM (int c);
	bool state_IDENT (int c);
	bool state_KW (int c);
	bool state_ASSIGN (int c);
	bool state_STR (int c);
	bool state_DELIM (int c);
	bool state_NEG (int c);
	bool state_FN (int c);
	bool state_COMMENT (int c);
	bool state_LABEL (int c);
};

#endif
