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
	ReadFrom 	rf;

	ListElem * create_elem (const Lex & l);
	void add_to_list (const Lex & l);
public:
	LexList (const char * path);
	~LexList ();

	void save_list ();

	Lex * get_lex_from_list ();

	void print_list ();

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
		NEQ, 
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
	bool state_NEQ (int c);
	bool state_FN (int c);
	bool state_COMMENT (int c);
	bool state_LABEL (int c);
};

#endif
