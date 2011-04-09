#ifndef _PARSER_HPP_
#define _PARSER_HPP_


#include "lexer.hpp"
#include "tables.hpp"


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

	
	void get_lex ();
	int look (type_of_lex type_lex, const type_of_lex * table);
	
public:
	Parser ();

	void analyze (LexList * ll);
};


#endif
