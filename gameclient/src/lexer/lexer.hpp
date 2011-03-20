#ifndef _LEXER_HPP_
#define _LEXER_HPP_

enum type_of_lex;

class Lex {
	type_of_lex t_lex;
	int v_lex;
public:
	Lex (type_of_lex t = LEX_NULL, int v = 0);
	type_of_lex get_type ();
	int get_value ();
	void print (); 
};

class Scanner {

public:
	Scanner ();
};

#endif
