#include "parser.hpp"
#include "../exeption/exeption.hpp"
#include <stdlib.h>


Parser:: Parser (const char * path)
	: cur_lex (), ll (path), table ()
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
	cur_lex = ll.get_lex_from_list () ;
	if ( cur_lex.type == LEX_NULL ) {
		throw LexExeption ("End of file.", cur_lex);
	}

}


void Parser:: analyze ()
{
	ll.save_list ();
	ll.print_list ();

	printf ("Begin parse.\n");

	get_lex ();
	B ();

	printf ("Syntax analyze:\tOK.\n"); 
}


void Parser:: B ()
{
	if ( cur_lex.type != LEX_BEGIN ) {
		throw LexExeption ("Expected 'begin'.", cur_lex);
	}
	
	get_lex ();

	C ();

	if ( cur_lex.type != LEX_FIN ) {
		throw LexExeption ("Expected ';'.", cur_lex);
	}
	
	get_lex ();

	while ( cur_lex.type != LEX_END ) {

		C ();

		if ( cur_lex.type != LEX_FIN ) {
			throw LexExeption ("Expected ';'", cur_lex);
		} 

		get_lex ();
	}
}

void Parser:: C ()
{
	if ( cur_lex.type == LEX_IF ) {
		get_lex ();
		ifthenelse ();
	}
	else if ( cur_lex.type == LEX_WHILE ) {
		get_lex ();
		whiledo ();
	}
	else if ( cur_lex.type == LEX_ID ) {
		get_lex ();
		assign ();
	}
	else if ( cur_lex.type == LEX_ARRAY ) {
		get_lex ();
		array ();
		assign ();
	}
	else if ( cur_lex.type == LEX_BEGIN ) {
		B ();
		get_lex ();
	}
	else if ( cur_lex.type == LEX_LABEL ) {
		get_lex ();
	}
	else if ( cur_lex.type == LEX_GOTO ) {
		get_lex ();

		if ( cur_lex.type == LEX_LABEL ) {
			get_lex ();
		}
		else {
			throw LexExeption ("Error in label of goto", cur_lex);
		}
	}
	else if ( 	cur_lex.type == LEX_BUY 	|| 
			cur_lex.type == LEX_SELL 	|| 
			cur_lex.type == LEX_PROD 	||
			cur_lex.type == LEX_BUILD 	|| 
			cur_lex.type == LEX_TURN 	|| 
			cur_lex.type == LEX_PRINT 
		) 
	{
		W ();
	}
	else {
		throw LexExeption ("Invdalid left-handed expression.", cur_lex);
	}
}


void Parser:: D ()
{
	E ();

	if (	cur_lex.type == LEX_EQ || 
		cur_lex.type == LEX_GREATER || 
		cur_lex.type == LEX_LESS) 
	{
		get_lex ();
		E ();
	}
}


void Parser:: E ()
{
	F ();

	while (	cur_lex.type == LEX_PLUS || 
		cur_lex.type == LEX_MINUS || 
		cur_lex.type == LEX_OR) 
	{
		get_lex ();
		F ();
	}
}


void Parser:: F ()
{
	G ();

	while (	cur_lex.type == LEX_MULTIPLY || 
		cur_lex.type == LEX_DIVISION || 
		cur_lex.type == LEX_AND) 
	{
		get_lex ();
		G ();
	}
}


void Parser:: G ()
{
	if ( cur_lex.type == LEX_ID ) {
		get_lex ();	
	}
	else if ( cur_lex.type == LEX_NUM ) {
		get_lex ();	
	}
	else if ( cur_lex.type == LEX_NEQ ) {
		get_lex ();
		G ();
	}
	else if ( cur_lex.type == LEX_LPAREN ) {
		
		get_lex ();
		D ();
		
		if ( cur_lex.type != LEX_RPAREN ) {
			throw LexExeption ("Expected ')'", cur_lex);
		}
		
		get_lex ();
	}
	else if ( cur_lex.type == LEX_ARRAY ) {
		get_lex ();
		array ();
	}
	else if ( cur_lex.type == LEX_RAW ) {
		get_lex ();
		Z ();	
	}
	else
	{
		throw LexExeption ("Unknown symbol of expression.", cur_lex);
	}

}


void Parser:: W ()
{
	if ( cur_lex.type == LEX_BUY ) {
		get_lex ();

		lparen ();

		D ();

		comma ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_SELL ) {
		get_lex ();

		lparen ();

		D ();

		comma ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_PROD ) {
		get_lex ();

		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_BUILD ) {
		get_lex ();

		lparen ();

		rparen ();
	}
	else if (cur_lex.type == LEX_TURN ) {
		get_lex ();

		lparen ();

		rparen ();
	}
	else if (cur_lex.type == LEX_PRINT ) {
		get_lex ();

		lparen ();

		L ();

		rparen ();
	}
	else {
		throw LexExeption ("Syntax error. Not allowed expression.", 
								cur_lex);
	}
}


void Parser:: Z ()
{
	if ( cur_lex.type == LEX_CUR_MONTH ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_PLAYERS ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_ACTIVE_PLAYERS ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_SUPPLY ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_RAW_PRICE ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_DEMAND ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_PRODUCTION_PRICE ) {
		lparen ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_MONEY ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_RAW ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_PRODUCTION ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_FACTORIES ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_AUTO_FACTORIES ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_MANUFACTURED ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_RESULT_RAW_SOLD ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_RESULT_RAW_PRICE ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_RESULT_PROD_BOUGHT ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_RESULT_PROD_PRICE ) {
		lparen ();

		D ();

		rparen ();
	}
	else 
	{
		throw LexExeption ("Not found function with return.", cur_lex);
	}
}


void Parser:: L ()
{
	elem ();
	
	if ( cur_lex.type == LEX_COMMA ) {
		get_lex ();
		L ();
	}
	
}


void Parser:: elem ()
{
	if ( cur_lex.type == LEX_STR ) {
		get_lex ();
	}
	else if ( 
			cur_lex.type == LEX_NUM || 
		  	cur_lex.type == LEX_ID || 
			cur_lex.type == LEX_ARRAY ||
		  	look (cur_lex.type , table.lex_function) != 0
		)
	{
		D ();	
	}
	else {
		throw LexExeption ("Expected expression or string.", cur_lex);
	}
}


void Parser:: assign ()
{
	if ( cur_lex.type == LEX_ASSIGN ) {
		get_lex ();
		D ();
	}
	else {
		throw LexExeption ("Expected assign", cur_lex);
	}
}


void Parser:: lparen () 
{
		if ( cur_lex.type != LEX_LPAREN ) {
			throw LexExeption ("Expected '('", cur_lex);
		}
		get_lex ();
}


void Parser:: rparen () 
{
		if ( cur_lex.type != LEX_RPAREN ) {
			throw LexExeption ("Expected ')'", cur_lex);
		}
		get_lex ();
}


void Parser:: comma () 
{
		if ( cur_lex.type != LEX_COMMA ) {
			throw LexExeption ("Expected ','", cur_lex);
		}
		get_lex ();
}


void Parser:: ifthenelse ()
{
		D ();

		if ( cur_lex.type == LEX_THEN ) {
			get_lex ();
			C ();	

			if ( cur_lex.type == LEX_ELSE ) {
				get_lex ();
				C ();
			}
		}
		else {
			throw LexExeption ("Expected 'then'", cur_lex);
		}
}


void Parser:: whiledo ()
{
		D ();

		if ( cur_lex.type == LEX_DO ) {
			get_lex ();
			C ();
		}
		else {
			throw LexExeption ("Expected 'do'.", cur_lex);
		}

}


void Parser:: array ()
{
		if ( cur_lex.type == LEX_LBRACKET ) {
			
			get_lex ();
			D ();

			if ( cur_lex.type == LEX_RBRACKET ) {
				get_lex ();
			}
			else {
				throw LexExeption ("Expected ']'", cur_lex);
			}

		}
		else {
			throw LexExeption ("Expected '['.", cur_lex);
		}
}
