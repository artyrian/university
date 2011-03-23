#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include "../buffer/buffer.hpp"
#include "tables.hpp"

class Lex {
	type_of_lex	t_lex;
	int 		v_lex;
	int		str_n;
public:
	Lex (ink k, type_of_lex t, int v);
	type_of_lex get_type ();
	int get_value ();
	int get_str_n ();
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
					COMMENT
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
	Lex H ();
	Lex NUM ();
	Lex IDENT ();
	Lex KW ();
	Lex ASSIGN ();
	Lex STR ();
	Lex DELIM ();
	Lex NEQ ();
	Lex FN ();
	Lex COMMENT ();
};

#endif
