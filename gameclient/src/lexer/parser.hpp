#ifndef _PARSER_HPP_
#define _PARSER_HPP_


#include "lexer.hpp"
#include "tables.hpp"
#include "rpn.hpp"
#include "../robot/game.hpp"


class Parser {
	Lex cur_lex;
	LexList	* lexl;
	TableLexem * table;
	TableLabel label;
	TableLabel labelgoto;
	Game * robot;

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
	void fill_while_labels (int, int);
	void add_address_id (int value);
	void for_loop ();
	void create_for_labels ();
	void fill_for_labels (int, int, int, int, int, int, int, int);
	void array (bool var);
	void assign ();
	void lparen ();
	void rparen ();
	void comma ();
	void stringelem ();
	void gotolabel ();
	void create_goto_label ();
	void add_label (const Lex & lex);
	void body ();
	void arg0 ();
	void arg1 ();
	void arg2 ();
	void add_switch_D (type_of_lex type);
	void add_switch_E (type_of_lex type);
	void add_switch_F (type_of_lex type);
	void check_labels ();
	
	char * newline;
	void paste_newline ();
	
	void get_lex ();
	int look (type_of_lex type_lex, 
		const type_of_lex * table
	);
public:
	Parser (LexList * ll, Game * robot_ptr);
	~Parser ();

	PolizList rpn;

	void analyze ();
};


#endif
