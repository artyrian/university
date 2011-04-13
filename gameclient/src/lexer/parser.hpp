#ifndef _PARSER_HPP_
#define _PARSER_HPP_


#include "lexer.hpp"
#include "tables.hpp"
#include "rpn.hpp"


class Parser {
	Lex cur_lex;
	LexList	* lexl;
	TableLexem * table;
	TableLabel label;

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
	void create_if_labels ();
	void fill_if_labels (int place1);
	void whiledo ();
	void create_while_labels ();
	void fill_while_labels (int place1, int place2);
	void add_address_id (int value);
	void array ();
	void assign ();
	void lparen ();
	void rparen ();
	void comma ();
	void stringelem ();
	void gotolabel ();
	void fill_goto_label (int place1);
	void add_label (const Lex & lex, int place);
	void body ();
	void arg0 ();
	void arg1 ();
	void arg2 ();
	void add_switch_D (type_of_lex type);
	void add_switch_E (type_of_lex type);
	void add_switch_F (type_of_lex type);

	
	void get_lex ();
	int look (type_of_lex type_lex, 
		const type_of_lex * table
	);
public:
	Parser ();
	~Parser ();

	PolizList rpn;

	void analyze (LexList * ll);
};


#endif
