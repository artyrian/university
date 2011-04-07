#include "parser.hpp"
#include "../exception/exception.hpp"
#include <stdlib.h>


Parser:: Parser ()
	: tbl ()
{
}


Parser:: ~Parser ()
{
}


int Parser:: look (type_of_lex type_lex, const type_of_lex * table)
{
	int i = 1;

	while ( table [i] != LEX_NULL ) {
		if ( table [i] == type_lex ) {
			return i;
		}
		++i;
	}

	return 0;
}


void Parser:: get_lex ()
{
	Lex * tmp_lex = lexl->get_lex_from_list () ;
	cur_lex = new Lex;
	cur_lex->strnum = tmp_lex->strnum;
	cur_lex->type = tmp_lex->type;
	cur_lex->value = tmp_lex->value;

	if ( cur_lex->type == LEX_NULL ) {
		throw LexException ("End of file.", *cur_lex);
	}

}


void Parser:: analyze (LexList * ll)
{
	lexl = ll;
	printf ("Begin parse.\n");

	get_lex ();
	O ();

	printf ("Syntax analyze:\tOK.\n"); 
}


void Parser:: O ()
{
	if ( cur_lex->type != LEX_BEGIN ) {
		throw LexException ("Expected 'begin'.", *cur_lex);
	}

	get_lex ();

	B ();
}


void Parser:: B ()
{
	C ();

	if ( cur_lex->type != LEX_FIN ) {
		throw LexException ("Expected ';'.", *cur_lex);
	}
	
	get_lex ();

	while ( cur_lex->type != LEX_END ) {

		C ();

		if ( cur_lex->type != LEX_FIN ) {
			throw LexException ("Expected ';'", *cur_lex);
		} 

		get_lex ();
	}	// while not found '}'
}


void Parser:: C ()
{
	if ( cur_lex->type == LEX_IF ) {
		ifthen ();
	}
	else if ( cur_lex->type == LEX_WHILE ) {
		whiledo ();
	}
	else if ( cur_lex->type == LEX_ID ) {
		get_lex ();
		assign ();
	}
	else if ( cur_lex->type == LEX_ARRAY ) {
		array ();
		assign ();
	}
	else if ( cur_lex->type == LEX_BEGIN ) {
		body ();
	}
	else if ( cur_lex->type == LEX_LABEL ) {
		get_lex ();
	}
	else if ( cur_lex->type == LEX_GOTO ) {
		gotolabel ();
	}
	else if ( look (cur_lex->type, tbl.lex_action) ) 
	{
		W ();
	}
	else {
		throw LexException ("Invdalid left-handed expression.", *cur_lex);
	}
} // C();


void Parser:: D ()
{
	E ();

	if (	cur_lex->type == LEX_EQ || 
		cur_lex->type == LEX_GREATER || 
		cur_lex->type == LEX_LESS) 
	{
		get_lex ();
		E ();
	}
}


void Parser:: E ()
{
	F ();

	while (	cur_lex->type == LEX_PLUS || 
		cur_lex->type == LEX_MINUS || 
		cur_lex->type == LEX_OR) 
	{
		get_lex ();
		F ();
	}
}


void Parser:: F ()
{
	G ();

	while (	cur_lex->type == LEX_MULTIPLY || 
		cur_lex->type == LEX_DIVISION || 
		cur_lex->type == LEX_AND) 
	{
		get_lex ();
		G ();
	}
}


void Parser:: G ()
{
	if ( cur_lex->type == LEX_ID ) {
		get_lex ();	
	}
	else if ( cur_lex->type == LEX_NUM ) {
		get_lex ();	
	}
	else if ( cur_lex->type == LEX_NEQ ) {
		get_lex ();
		G ();
	}
	else if ( cur_lex->type == LEX_LPAREN ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_ARRAY ) {
		array ();
	}
	else if ( look (cur_lex->type, tbl.lex_function) ) {
		Z ();	
	}
	else
	{
		throw LexException ("Unknown symbol of expression.", *cur_lex);
	}

} // G();


void Parser:: W ()
{
	if ( cur_lex->type == LEX_BUY ) {
		get_lex ();

		lparen ();

		D ();

		comma ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_SELL ) {
		get_lex ();

		lparen ();

		D ();

		comma ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_PROD ) {
		get_lex ();

		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_BUILD ) {
		get_lex ();

		lparen ();

		rparen ();
	}
	else if (cur_lex->type == LEX_TURN ) {
		get_lex ();

		lparen ();

		rparen ();
	}
	else if (cur_lex->type == LEX_PRINT ) {
		get_lex ();

		lparen ();

		S ();

		rparen ();
	}
	else {
		throw LexException ("Syntax error. Not allowed expression.", *cur_lex);
	}
}


void Parser:: Z ()
{
	if ( cur_lex->type == LEX_CUR_MONTH ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_PLAYERS ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_ACTIVE_PLAYERS ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_SUPPLY ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_RAW_PRICE ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_DEMAND ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_PRODUCTION_PRICE ) {
		get_lex ();
		lparen ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_MONEY ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_RAW ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_PRODUCTION ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_FACTORIES ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_AUTO_FACTORIES ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_MANUFACTURED ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_RESULT_RAW_SOLD ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_RESULT_RAW_PRICE ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_RESULT_PROD_BOUGHT ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex->type == LEX_RESULT_PROD_PRICE ) {
		get_lex ();
		lparen ();

		D ();

		rparen ();
	}
	else 
	{
		throw LexException ("Not found function with return.", *cur_lex);
	}
}


void Parser:: S ()
{
	stringelem ();
	
	if ( cur_lex->type == LEX_COMMA ) {
		get_lex ();
		S ();
	}
	
}


void Parser:: stringelem ()
{
	if ( cur_lex->type == LEX_STR ) {
		get_lex ();
	}
	else if ( 
		cur_lex->type == LEX_NUM || 
		cur_lex->type == LEX_ID || 
		cur_lex->type == LEX_ARRAY ||
		look (cur_lex->type , tbl.lex_function) != 0
		)
	{
		D ();	
	}
	else {
		throw LexException ("Expected expression or string.", *cur_lex);
	}
}


void Parser:: assign ()
{
	if ( cur_lex->type == LEX_ASSIGN ) {
		get_lex ();
		D ();
	}
	else {
		throw LexException ("Expected assign", *cur_lex);
	}
}


void Parser:: lparen () 
{
	if ( cur_lex->type != LEX_LPAREN ) {
		throw LexException ("Expected '('", *cur_lex);
	}
	get_lex ();
}


void Parser:: rparen () 
{
	if ( cur_lex->type != LEX_RPAREN ) {
		throw LexException ("Expected ')'", *cur_lex);
	}
	get_lex ();
}


void Parser:: comma () 
{
	if ( cur_lex->type != LEX_COMMA ) {
		throw LexException ("Expected ','", *cur_lex);
	}
	get_lex ();
}


void Parser:: ifthen ()
{
	if ( cur_lex->type != LEX_IF ) {
		throw LexException ("Must be if there. Source code.", *cur_lex);
	}

	get_lex ();

	D ();

	if ( cur_lex->type == LEX_THEN ) {
		get_lex ();
		C ();	
	}
	else {
		throw LexException ("Expected 'then'", *cur_lex);
	}
}


void Parser:: whiledo ()
{
	if ( cur_lex->type != LEX_WHILE ) {
		throw LexException ("Must be while. Source code", *cur_lex);
	}

	get_lex ();

	D ();

	if ( cur_lex->type == LEX_DO ) {
		get_lex ();
		C ();
	}
	else {
		throw LexException ("Expected 'do'.", *cur_lex);
	}
}


void Parser:: array ()
{
	if ( cur_lex->type != LEX_ARRAY ) {
		throw LexException ("Must be array. Source code", *cur_lex);
	}

	get_lex ();

	if ( cur_lex->type == LEX_LBRACKET ) {
		
		get_lex ();
		D ();

		if ( cur_lex->type == LEX_RBRACKET ) {
			get_lex ();
		}
		else {
			throw LexException ("Expected ']'", *cur_lex);
		}

	}
	else {
		throw LexException ("Expected '['.", *cur_lex);
	}
}


void Parser:: gotolabel ()
{
	if ( cur_lex->type != LEX_GOTO ) {
		throw LexException ("Must be 'goto'. Source code.", *cur_lex);
	}
	
	get_lex ();

	if ( cur_lex->type == LEX_LABEL ) {
		get_lex ();
	}
	else {
		throw LexException ("Error in label of goto", *cur_lex);
	}
}


void Parser:: body () {
	get_lex ();
	B ();
	get_lex ();
}
