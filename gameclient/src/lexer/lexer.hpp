#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include "../buffer/buffer.hpp"
#include "tables.hpp"

class Lex {
	type_of_lex	t_lex;
	int 		v_lex;
	int		str_n;
public:
	Lex ();
	Lex (int n);
	Lex (int k, type_of_lex t, int v);

	type_of_lex get_type () const ;
	int get_value () const;
	int get_str_n () const;
	void print () const; 
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

	int isdelim (int c);
	int look (const char * buf, const char ** list);
public:
	Scanner ();
	Lex feed_symbol (int c);
	~Scanner ();
private:
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
