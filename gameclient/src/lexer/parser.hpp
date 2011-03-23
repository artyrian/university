#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "lexlist.hpp"
#include "lexer.hpp"

class Parser {
	Lex		cur_lex;
	type_of_lex	c_type;
	int		c_val;
	LexList		le;

	void B ();
	void C ();
	void D ();
	void E ();
	void F ();
	void G ();
	void W ();
	void Z ();

	void get_lex ();
	
	void colon ();
	void assign ();
	void lparen ();
	void rparen ();
	void comma ();
public:
	Parser (const char * path);
	void analyze ();
};

#endif

void Parser:: assign ()
{
	get_lex ();
	if ( c_type == LEX_ASSIGN ) {
		get_lex ();
		D ();
		
		colon ();
	}
	else {
		throw LexExeption ("Expext assign", cur_lex);
	}
}
