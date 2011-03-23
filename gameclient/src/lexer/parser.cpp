#include "parser.hpp"
#include "exeption.hpp"

Parser:: Parser (const char * path)
	: le (path)
{
	le.save ();
}

void Parser:: get_lex ()
{
	cur_lex = le.get_lex () ;
	c_type = cur_lex.get_type (); 
	c_val = cur_lex.get_val ();

	if ( cur_lex.get_type == 0 ) {
		throw LexExeption ("End of file." cur_lex);
	}
}

void Parser:: analyze ()
{
	printf ("Begin parse.\n");

	get_lex ();
	B ();

	printf ("End parser.\n"); 
}


void Parser:: B ()
{
	if ( c_type != LEX_BEGIN ) {
		throw LexExeption ("'{' absent.", cur_lex);
	}
	
	get_lex ();
	C ();

	colon ();

	while ( c_type != LEX_END ) {

		C ();

		colon ();
	}
}

void Parser:: C ()
{
	if ( c_type == LEX_IF ) {
		get_lex ();
		D ();

		if ( c_type == LEX_THEN ) {

			get_lex ();
			if ( c_type == LEX_BEGIN ) {
				B ();
			}
			else {
				throw LexExeption ("Exepted '{'.", cur_lex);
			}

			get_lex ();
			if ( c_type == LEX_ELSE ) {
		
				get_lex ();
				if ( c_type == LEX_BEGIN ) {
					B ();
				}
				else {
					throw LexExeption ("Exepted '{'.", cur_lex);
				}
			}
		}
		else {
			throw LexExeption ("Expected 'then'", cur_lex);
		}
	}
	else if ( c_type == LEX_WHILE ) {
		get_lex ();
		D ();

		if ( c_type == LEX_DO ) {

			get_lex ();
			if ( c_type == LEX_BEGIN ) {
				B ();
			}
			else {
				throw LexExeption ("Expected '{'.", cur_lex);
			}
		}
		else {
			throw LexExeption ("Expected 'do'.", cur_lex);
		}
	}
	else if ( c_type == LEX_ID ) {
		
		assign ();
	}
	else if ( c_type == LEX_ARRAY ) {
		
		get_lex ();
		if ( c_type == LEX_LBRACKET ) {
			
			get_lex ();
			D ();

			if ( c_type == LEX_RBRACKET ) {
				assign ();
			}
			else {
				throw LexExeption ("Exepted ']'", cur_lex);
			}

		}
		else {
			throw LexExeption ("Expected '['.", cur_lex);
		}
	}
	else {
		throw LexExeption ("Syntax error.", cur_lex);
	}

}

void Parser:: D ()
{
	E ();
	
	if 	( 	
			c_type == LEX_EQ 	||	
			c_type == LEX_GREATER	|| 
			c_type == LEX_LESS 	||
			c_type == LEX_LEQ	||
			c_type == LEX_GEQ 	||
			c_type == LEX_LG 
		)
	{
		
		get_lex ();
		E ();
	}
	else {
		throw LexExeption ("Expected compare." cur_lex);
	}

	get_lex ();
}

void Parser:: E ()
{
	F ();

	get_lex ();
	if 	( 	
			c_type == LEX_PLUS 	|| 
			c_type == LEX_MINUS 	|| 
			c_type == LEX_OR 
		) 
	{
		
		get_lex ();
		F ();
	}
	else {
		throw LexExeption ("Expected operator." cur_lex);
	}

	get_lex ();
}

void Parser:: F ()
{
	G ();

	get_lex ();
	if 	( 	
			c_type == LEX_MULTIPLY	|| 
			c_type == LEX_DIVISION	|| 
			c_type == LEX_AND 
		) 
	{
		
		get_lex ();
		G ();
	}
	else {
		throw LexExeption ("Expected operator." cur_lex);
	}
}

void Parser:: G ()
{
	if ( c_type == LEX_ID ) {
		get_lex ();	
	}
	else if ( c_type == LEX_NUM ) {
		get_lex ();	
	}
	else if ( c_type == LEX_NEQ ) {
		get_lex ();
		G ();
	}
	else if ( c_type == LEX_LPAREN ) {
		
		get_lex ();
		D ();
		
		if ( c_type != LEX_RPAREN ) {
			throw LexExeption ("Expected ')'", cur_lex);
		}
		
		get_lex ();
	}
	else if ( c_type == LEX_ARRAY ) {
		get_lex ();
		A ();
	}
	else 
		get_lex ();
		Z ();	
	}

}

void Parser:: W ()
{
	if ( c_type == LEX_BUY ) {
		get_lex ();

		lparen ();

		D ();

		comma ();

		D ();

		rparen ();
	}
	else if ( c_type == LEX_SELL ) {
		get_lex ();

		lparen ();

		D ();

		comma ();

		D ();

		rparen ();
	}
	else if ( c_type == LEX_PROD ) {
		get_lex ();

		lparen ();

		D ();

		rparen ();
	}
	else if (c_type == LEX_TURN ) {
		get_lex ();

		lparen ();

		rparen ();
	}
	else {
		throw LexExeption ("Unkown function.", cur_lex);
	}
}

void Parser:: Z ()
{
	printf ("Not realizied.\n");
	if ( 1 ) {
		lparen ();

		rparen ();
	}
	else if ( 0 ) {
		lparen ();

		D ();

		rparen ();
	}
}

void Parser:: A ()
{
	if ( c_type != LEX_LBRACKET ) {
		throw LexExeption ("Expected '['.", cur_lex);
	}

	D ();

	if ( c_type != LEX_RBRACKET ) {
		throw LexExeption ("Expected ']'.", cur_lex);
	}

	get_lex ();
}

void Parser:: colon ()
{
	if ( c_type != LEX_FIN ) {
		throw LexExeption ("Exeption ';'", cur_lex);
	}

	get_lex ();
}

void Parser:: assign ()
{
	if ( c_type == LEX_ASSIGN ) {
		get_lex ();
		D ();
		
		colon ();
	}
	else {
		throw LexExeption ("Expext assign", cur_lex);
	}
}

void Parser:: lparen () 
{
		if ( c_type != LEX_LPAREN ) {
			throw LexExeption ("Expected '('", cur_lex);
		}
		get_lex ();
}

void Parser:: rparen () 
{
		if ( c_type != LEX_RPAREN ) {
			throw LexExeption ("Expected ')'", cur_lex);
		}
		get_lex ();
}

void Parser:: comma () 
{
		if ( c_type != LEX_COMMA ) {
			throw LexExeption ("Expected ','", cur_lex);
		}
		get_lex ();
}
