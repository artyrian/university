#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include "buffer.hpp"

enum type_of_lex;

class Lex {
	type_of_lex	t_lex;
	int 		v_lex;
public:
	Lex (type_of_lex t = LEX_NULL, int v = 0);
	type_of_lex get_type ();
	int get_value ();
	void print (); 
};

class Scanner {
	enum			state { H, I, N, E };
	static char *		TW[];
	static type_of_lex	words[];

	Buffer * buf;
public:
	Scanner ();
	get_lex ();
};

#endif
