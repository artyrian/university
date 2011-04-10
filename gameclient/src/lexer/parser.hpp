#ifndef _PARSER_HPP_
#define _PARSER_HPP_


#include "lexer.hpp"
#include "tables.hpp"
#include "rpn.hpp"


class Parser {
	Lex cur_lex;
	LexList	* lexl;

	void O ();
	void B ();
	void C ();
	void D ();
	void E ();
	void F ();
	void G ();
	void W ();
	void Z ();
	void S ();

	void ifthen ();
	void whiledo ();
	void array ();
	void assign ();
	void lparen ();
	void rparen ();
	void comma ();
	void stringelem ();
	void gotolabel ();
	void body ();
	void arg0 ();
	void arg1 ();
	void arg2 ();

	
	void get_lex ();
	int look (type_of_lex type_lex, const type_of_lex * table);
	
public:
	Parser ();
	~Parser ();

	PolizList rpn;

	void analyze (LexList * ll);
};


#endif
