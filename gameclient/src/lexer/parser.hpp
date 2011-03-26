#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "lexer.hpp"
#include "poliz.hpp"

class Parser {
	Lex		cur_lex;
	type_of_lex	c_type;
	int		c_val;
	LexList		ll;
	TableLexem	table;

	void B ();
	void C ();
	void D ();
	void E ();
	void F ();
	void G ();
	void W ();
	void Z ();
	void L ();

	void get_lex ();

	int look (type_of_lex type_lex, const type_of_lex * table);
	
	void ifthenelse ();
	void whiledo ();
	void array ();
	void assign ();
	void lparen ();
	void rparen ();
	void comma ();
	void elem ();
public:
	Poliz	prg;			// not realized yet.
	Parser (const char * path);
	void analyze ();
};

#endif
