#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "lexlist.hpp"
#include "lexer.hpp"

class Parser {
	Lex		cur_lex;
	type_of_lex	c_type;
	int		c_val;
	LexList		ll;

	void B ();
	void C ();
	void D ();
	void E ();
	void F ();
	void G ();
	void W ();
	void Z ();

	void get_lex ();
	
	void ifthenelse ();
	void whiledo ();
	void array ();
	void assign ();
	void lparen ();
	void rparen ();
	void comma ();
public:
	Parser (const char * path);
	void analyze ();
};

#endif
