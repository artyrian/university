#include "parser.hpp"
#include "../exception/exception.hpp"
#include <stdlib.h>


Parser:: Parser ()
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
	cur_lex = lexl->get_lex_from_list () ;

	if ( cur_lex.type == LEX_NULL ) {
		throw LexException ("End of file.", cur_lex);
	}

}


void Parser:: analyze (LexList * ll)
{
	lexl = ll;
	table = ll->get_pointer_to_table ();
	printf ("Begin parse.\n");

	get_lex ();
	O ();

	printf ("Syntax analyze:\tOK.\n"); 
}


void Parser:: O ()
{
	if ( cur_lex.type != LEX_BEGIN ) {
		throw LexException ("Expected 'begin'.", cur_lex);
	}

	get_lex ();

	B ();

}


void Parser:: B ()
{
	C ();

	if ( cur_lex.type != LEX_FIN ) {
		throw LexException ("Expected ';'.", cur_lex);
	}
	
	get_lex ();

	while ( cur_lex.type != LEX_END ) {

		C ();

		if ( cur_lex.type != LEX_FIN ) {
			throw LexException ("Expected ';'", cur_lex);
		} 

		get_lex ();
	}	// while not found '}'
}


void Parser:: C ()
{
	if ( cur_lex.type == LEX_IF ) {
		ifthen ();
	}
	else if ( cur_lex.type == LEX_WHILE ) {
		whiledo ();
	}
	else if ( cur_lex.type == LEX_ID ) {
		add_address_id (cur_lex.value);
		get_lex ();
		assign ();
	}
	else if ( cur_lex.type == LEX_ARRAY ) {
		array ();
		assign ();
	}
	else if ( cur_lex.type == LEX_BEGIN ) {
		body ();
	}
	else if ( cur_lex.type == LEX_LABEL ) {
		add_label (cur_lex, rpn.get_size ());
		get_lex ();
	}
	else if ( cur_lex.type == LEX_GOTO ) {
		gotolabel ();
	}
	else if ( look (cur_lex.type, TableLexem:: lex_action) ) 
	{
		W ();
	}
	else {
		throw LexException ("Invdalid left-handed expression.", cur_lex);
	}
} // C();


void Parser:: D ()
{

	E ();

	if (	cur_lex.type == LEX_EQ || 
		cur_lex.type == LEX_GREATER || 
		cur_lex.type == LEX_LESS) 
	{
		type_of_lex type = cur_lex.type;

		get_lex ();
		E ();

		add_switch_D (type);
	}
}


void Parser:: E ()
{
	F ();

	while (	cur_lex.type == LEX_PLUS || 
		cur_lex.type == LEX_MINUS || 
		cur_lex.type == LEX_OR) 
	{
		type_of_lex type = cur_lex.type;

		get_lex ();
		F ();

		add_switch_E (type);
	}
}


void Parser:: F ()
{
	G ();

	while (	cur_lex.type == LEX_MULTIPLY || 
		cur_lex.type == LEX_DIVISION || 
		cur_lex.type == LEX_AND) 
	{
		type_of_lex type = cur_lex.type;

		get_lex ();
		G ();

		add_switch_F (type);
	}
}


void Parser:: G ()
{
	if ( cur_lex.type == LEX_ID ) {
		int value = cur_lex.value;

		get_lex ();	

		printf ("P_ID\t");
		int res = table->ident [value].get_value ();
		rpn.add_to_list ( new PolizInt ( res ) ); 
	}
	else if ( cur_lex.type == LEX_NUM ) {
		int value = cur_lex.value;

		get_lex ();	

		printf ("P_NUM\t");
		rpn.add_to_list ( new PolizInt (value) );
	}
	else if ( cur_lex.type == LEX_NEG ) {
		get_lex ();

		G ();
		printf ("P_NEG");
		rpn.add_to_list ( new PolizFunNeg () );
	}
	else if ( cur_lex.type == LEX_LPAREN ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_ARRAY ) {
		array ();
	}
	else if ( look (cur_lex.type, TableLexem:: lex_function) ) {
		Z ();	
	}
	else
	{
		throw LexException ("Unknown symbol of expression.", cur_lex);
	}

} // G();

void Parser:: W ()
{
	if ( cur_lex.type == LEX_BUY ) {
		arg2 ();

		printf ("P_BUY(2)\t");
		rpn.add_to_list ( new PolizFunBuy () );
	}
	else if ( cur_lex.type == LEX_SELL ) {
		arg2 ();

		printf ("P_SELL(2)\t");
		rpn.add_to_list ( new PolizFunSell () );
	}
	else if ( cur_lex.type == LEX_PROD ) {
		arg1 ();

		printf ("P_PROD(1)\t");
		rpn.add_to_list ( new PolizFunProd () );
	}
	else if ( cur_lex.type == LEX_BUILD ) {
		arg0 ();

		printf ("P_BUILD(0)\t");
		rpn.add_to_list ( new PolizFunBuild () );
	}
	else if (cur_lex.type == LEX_TURN ) {
		arg0 ();

		printf ("P_TURN(0)\t");
		rpn.add_to_list ( new PolizFunTurn () );
	}
	else if (cur_lex.type == LEX_PRINT ) {
		Lex p_lex  = cur_lex;
		get_lex ();

		lparen ();
		S ();
		rparen ();

		printf ("P_PRINT(x)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else {
		throw LexException ("Syntax error. Not allowed expression.", 
			cur_lex
		);
	}
}


void Parser:: Z ()
{
	if ( cur_lex.type == LEX_CUR_MONTH ) {
		arg0 ();

		printf ("P_CUR_MONTH\t");
		rpn.add_to_list ( new PolizFunCurMonth () );
	}
	else if ( cur_lex.type == LEX_PLAYERS ) {
		arg0 ();

		printf ("P_PLAYERS\t");
		rpn.add_to_list ( new PolizFunPlayers () );
	}
	else if ( cur_lex.type == LEX_ACTIVE_PLAYERS ) {
		arg0 ();

		printf ("P_ACTIVE_PLAYERS\t");
		rpn.add_to_list ( new PolizFunActivePlayers () );
	}
	else if ( cur_lex.type == LEX_SUPPLY ) {
		arg0 ();

		printf ("P_SUPPLY\t");
		rpn.add_to_list ( new PolizFunSupply () );
	}
	else if ( cur_lex.type == LEX_RAW_PRICE ) {
		arg0 ();

		printf ("P_RAW_PRICE\t");
		rpn.add_to_list ( new PolizFunRawPrice () );
	}
	else if ( cur_lex.type == LEX_DEMAND ) {
		arg0 ();

		printf ("P_DEMAND\t");
		rpn.add_to_list ( new PolizFunDemand () );
	}
	else if ( cur_lex.type == LEX_PRODUCTION_PRICE ) {
		arg0 ();

		printf ("P_PRODUCTION_PRICE\t");
		rpn.add_to_list ( new PolizFunProductionPrice () );
	}
	else if ( cur_lex.type == LEX_MONEY ) {
		arg1 ();

		printf ("P_MONEY(1)\t");
		rpn.add_to_list ( new PolizFunMoney () );
	}
	else if ( cur_lex.type == LEX_RAW ) {
		arg1 ();

		printf ("P_RAW(1)\t");
		rpn.add_to_list ( new PolizFunRaw () );
	}
	else if ( cur_lex.type == LEX_PRODUCTION ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_PRODUCTION(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_FACTORIES ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_FACTORIES(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_AUTO_FACTORIES ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_AUTOFACTORIES(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_MANUFACTURED ) {
		Lex p_lex  = cur_lex;

		arg1 ();
		
		printf ("P_MANUFACTURED(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_RESULT_RAW_SOLD ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_RESULT_RAW_SOLD(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_RESULT_RAW_PRICE ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_RESULT_RAW_PRICE(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_RESULT_PROD_BOUGHT ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_RESULT_PROD_BOUGHT(1)\t");
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else if ( cur_lex.type == LEX_RESULT_PROD_PRICE ) {
		Lex p_lex  = cur_lex;

		arg1 ();

		printf ("P_RESULT_PROD_PRICE(1)\t");
	
		rpn.add_to_list ( new PolizTest (p_lex) );
	}
	else 
	{
		throw LexException ("Not found function with return.", cur_lex);
	}
}


void Parser:: S ()
{
	stringelem ();
	
	if ( cur_lex.type == LEX_COMMA ) {
		get_lex ();
		S ();
	}
	
}


void Parser:: stringelem ()
{
	if ( cur_lex.type == LEX_STR ) {
		int value =cur_lex.value;

		get_lex ();

		printf ("LES_STR\t");
		char * str = table->string [value].get_name ();
		rpn.add_to_list ( new PolizString (str) );
	}
	else if ( 
		cur_lex.type == LEX_NUM || 
		cur_lex.type == LEX_ID || 
		cur_lex.type == LEX_ARRAY ||
		look (cur_lex.type , TableLexem:: lex_function) != 0
		)
	{
		D ();	
	}
	else {
		throw LexException ("Expected expression or string.", cur_lex);
	}
}


void Parser:: assign ()
{
	if ( cur_lex.type == LEX_ASSIGN ) {
		get_lex ();
		D ();
		printf ("P_ASSIGN\t");
		rpn.add_to_list ( new PolizAssign () );
	}
	else {
		throw LexException ("Expected assign", cur_lex);
	}
}


void Parser:: lparen () 
{
	if ( cur_lex.type != LEX_LPAREN ) {
		throw LexException ("Expected '('", cur_lex);
	}
	get_lex ();
}


void Parser:: rparen () 
{
	if ( cur_lex.type != LEX_RPAREN ) {
		throw LexException ("Expected ')'", cur_lex);
	}
	get_lex ();
}


void Parser:: comma () 
{
	if ( cur_lex.type != LEX_COMMA ) {
		throw LexException ("Expected ','", cur_lex);
	}
	get_lex ();
}


void Parser:: ifthen ()
{
	if ( cur_lex.type != LEX_IF ) {
		throw LexException ("Must be if there. Source code.", cur_lex);
	}

	get_lex ();

	D ();

	int place1 = rpn.get_size ();

	create_if_labels ();

	if ( cur_lex.type == LEX_THEN ) {
		get_lex ();
		C ();	
	}
	else {
		throw LexException ("Expected 'then'", cur_lex);
	}

	fill_if_labels (place1);
}


void Parser:: create_if_labels ()
{
	printf ("POLIZ_LABEL\t");
	rpn.add_to_list ( new PolizLabel (0) );

	printf ("POLIZ_FGO\t");
	rpn.add_to_list ( new PolizOpGoFalse () );
}


void Parser:: fill_if_labels (int place1)
{
	printf ("[<--]\t");
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (rpn.get_size ())),
		place1
	);
}


void Parser:: whiledo ()
{
	if ( cur_lex.type != LEX_WHILE ) {
		throw LexException ("Must be while. Source code", cur_lex);
	}
	
	int place1 = rpn.get_size ();

	get_lex ();

	D ();

	int place2 = rpn.get_size ();

	create_while_labels ();

	if ( cur_lex.type == LEX_DO ) {
		get_lex ();
		C ();
	}
	else {
		throw LexException ("Expected 'do'.", cur_lex);
	}

	fill_while_labels (place1, place2);
}


void Parser:: create_while_labels ()
{
	printf ("POLIZ_LABEL\t");
	rpn.add_to_list ( new PolizLabel (0) );

	printf ("POLIZ_FGO\t");
	rpn.add_to_list ( new PolizOpGoFalse () );
}


void Parser:: fill_while_labels (int place1, int place2)
{
	printf ("POLIZ_LABEL\t");
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place1)) );

	printf ("P_GO\t");
	rpn.add_to_list ( new PolizOpGo () );

	printf ("[<--]\t");
	rpn.add_to_list ( new PolizLabel ( rpn.get_pointer (rpn.get_size ()) ),
		place2
	);
}


void Parser:: add_address_id (int value)
{
	printf ("P_ADDRESS\t");
	int * id = table->ident [value].get_address_id ();
	rpn.add_to_list ( new PolizVarAddress ( id ) );
}


void Parser:: array ()
{
	if ( cur_lex.type != LEX_ARRAY ) {
		throw LexException ("Must be array. Source code", cur_lex);
	}
	
	printf ("P_ID_ARR");
	rpn.add_to_list ( new PolizTest (Lex (0, POLIZ_ID_ARRAY)) );

	get_lex ();

	if ( cur_lex.type == LEX_LBRACKET ) {
		
		get_lex ();
		D ();

		if ( cur_lex.type == LEX_RBRACKET ) {
			get_lex ();
		}
		else {
			throw LexException ("Expected ']'", cur_lex);
		}

	}
	else {
		throw LexException ("Expected '['.", cur_lex);
	}
	
	printf ("P_ARR\t");
	rpn.add_to_list ( new PolizTest (Lex (0, LEX_ARRAY)) );
}


void Parser:: gotolabel ()
{
	if ( cur_lex.type != LEX_GOTO ) {
		throw LexException ("Must be 'goto'. Source code.", cur_lex);
	}
	
	get_lex ();

	Lex p_lex = cur_lex;
	if ( cur_lex.type == LEX_LABEL ) {
		get_lex ();
	}
	else {
		throw LexException ("Error in label of goto", cur_lex);
	}
	
	int place1 = label.look (p_lex.value);
	if ( place1 == 0 ) {
		throw LexException ("Not found label", p_lex);
	}

	fill_goto_label (place1);
}


void Parser:: fill_goto_label (int place1)
{
	printf ("P_LABEL\t");
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place1)) );

	printf ("P_GO\t");
	rpn.add_to_list ( new PolizOpGo () );
}


void Parser:: add_label (const Lex & cur_lex, int place)
{

	int i = label.put (cur_lex.value, place);

	if ( i == 0 ) {
		throw LexException ("You have double lable", cur_lex);
	}
}


void Parser:: body () {
	get_lex ();
	B ();
	get_lex ();
}


void Parser:: arg0 ()
{
	lparen ();
	rparen ();
}


void Parser:: arg1 ()
{
	get_lex ();
	lparen ();
	D ();
	rparen ();
}


void Parser:: arg2 ()
{
	get_lex ();
	lparen ();
	D ();
	comma ();
	D ();
	rparen ();
}


void Parser:: add_switch_D (type_of_lex type) 
{
	printf ("P___EQ/GREATER/LESS\t");

	if ( type == LEX_EQ ) {
		rpn.add_to_list ( new PolizFunEq () );
	}
	else if ( type == LEX_GREATER ) {
		rpn.add_to_list ( new PolizFunGreater () );
	}
	else if ( type == LEX_LESS ) {
		rpn.add_to_list ( new PolizFunLess () );
	}
	else {
		throw ("Must be EQ/GREATER/LESS. Source code");
	}
}


void Parser:: add_switch_E (type_of_lex type)
{
	printf ("P___PLUS/MINUS/OR\t");

	if ( type == LEX_PLUS ) {
		rpn.add_to_list ( new PolizFunPlus () );
	}
	else if ( type == LEX_MINUS ) {
		rpn.add_to_list ( new PolizFunMinus () );
	}
	else if ( type == LEX_OR) {
		rpn.add_to_list ( new PolizFunOr () );
	}
	else {
		throw ("Must be PLUS/MINUS/OR. Source error.");
	}
}


void Parser:: add_switch_F (type_of_lex type)
{
	printf ("P___MUL/DIV/AND\t");

	if ( type == LEX_MULTIPLY ) {
		rpn.add_to_list ( new PolizFunMul () );
	}	
	else if ( type == LEX_DIVISION ) {
		rpn.add_to_list ( new PolizFunDiv () );
	}
	else if ( type == LEX_AND ) {
		rpn.add_to_list ( new PolizFunAnd () );
	}
	else {
		throw ("Must be (MUL/DIV/AND. Source error.");
	}
}

