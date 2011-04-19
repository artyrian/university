#include "parser.hpp"
#include "../exception/exception.hpp"
#include <stdlib.h>
#include <string.h>


Parser:: Parser (LexList * ll)
{
	lexl = ll;
	newline = new char [2];
	strcpy (newline, "\n");
}


Parser:: ~Parser ()
{
	delete [] newline;
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


void Parser:: analyze ()
{
	table = lexl->get_pointer_to_table ();

	get_lex ();
	O ();
	
	check_labels ();
	printf ("Syntax analyze:\tOK.\n"); 
}


void Parser:: O ()
{
	if ( cur_lex.type != LEX_BEGIN ) {
		throw LexException ("Expected 'begin'.", cur_lex);
	}

	get_lex ();

	B ();
	
	rpn.add_to_list ( new PolizNop () );

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
	else if ( cur_lex.type == LEX_FOR ) {
		for_loop ();
	}
	else if ( cur_lex.type == LEX_ID ) {
		add_address_id (cur_lex.value);
		get_lex ();
		assign ();
	}
	else if ( cur_lex.type == LEX_ARRAY ) {
		array (true);
		assign ();
	}
	else if ( cur_lex.type == LEX_BEGIN ) {
		body ();
	}
	else if ( cur_lex.type == LEX_LABEL ) {
		add_label (cur_lex);
		get_lex ();
	}
	else if ( cur_lex.type == LEX_GOTO ) {
		gotolabel ();
	}
	else if ( look (cur_lex.type, TableLexem:: lex_action) != 0 ) 
	{
		W ();
	}
	else {
		throw LexException ("Invdalid left-handed expression.", 
			cur_lex
		);
	}
} // C();


void Parser:: D ()
{
	E ();

	if (  look (cur_lex.type, TableLexem :: lex_compare) != 0 ) {
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

		int * id = table->ident.index ( value )->get_address_value ();

		rpn.add_to_list ( new PolizVarInt ( id ) ); 
	}
	else if ( cur_lex.type == LEX_NUM ) {
		int value = cur_lex.value;

		get_lex ();	

		rpn.add_to_list ( new PolizInt ( value ) );
	}
	else if ( cur_lex.type == LEX_NEG ) {
		get_lex ();

		G ();
		rpn.add_to_list ( new PolizFunNeg () );
	}
	else if ( cur_lex.type == LEX_LPAREN ) {
		lparen ();

		D ();

		rparen ();
	}
	else if ( cur_lex.type == LEX_ARRAY ) {
		array (false);
	}
	else if ( look (cur_lex.type, TableLexem:: lex_function) ) {
		Z ();	
	}
	else
	{
		throw LexException ("Unknown symbol of expression.",
			cur_lex
		);
	}

} // G();


void Parser:: W ()
{
	if ( cur_lex.type == LEX_BUY ) {
		arg2 ();

		rpn.add_to_list ( new PolizFunBuy () );
	}
	else if ( cur_lex.type == LEX_SELL ) {
		arg2 ();

		rpn.add_to_list ( new PolizFunSell () );
	}
	else if ( cur_lex.type == LEX_PROD ) {
		arg1 ();

		rpn.add_to_list ( new PolizFunProd () );
	}
	else if ( cur_lex.type == LEX_BUILD ) {
		arg0 ();

		rpn.add_to_list ( new PolizFunBuild () );
	}
	else if (cur_lex.type == LEX_TURN ) {
		arg0 ();

		rpn.add_to_list ( new PolizFunTurn () );
	}
	else if (cur_lex.type == LEX_PRINT ) {
		get_lex ();

		lparen ();
		S ();
		paste_newline ();
		rparen ();
	}
	else {
		throw LexException (
			"Syntax error. Not allowed expression.", 
			cur_lex
		);
	}
}


void Parser:: Z ()
{
	if ( cur_lex.type == LEX_CUR_MONTH ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunCurMonth () );
	}
	else if ( cur_lex.type == LEX_PLAYERS ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunPlayers () );
	}
	else if ( cur_lex.type == LEX_ACTIVE_PLAYERS ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunActivePlayers () );
	}
	else if ( cur_lex.type == LEX_SUPPLY ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunSupply () );
	}
	else if ( cur_lex.type == LEX_RAW_PRICE ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunRawPrice () );
	}
	else if ( cur_lex.type == LEX_DEMAND ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunDemand () );
	}
	else if ( cur_lex.type == LEX_PRODUCTION_PRICE ) {
		arg0 ();
		rpn.add_to_list ( new PolizFunProductionPrice () );
	}
	else if ( cur_lex.type == LEX_MONEY ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunMoney () );
	}
	else if ( cur_lex.type == LEX_RAW ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunRaw () );
	}
	else if ( cur_lex.type == LEX_PRODUCTION ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunProduction () );
	}
	else if ( cur_lex.type == LEX_FACTORIES ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunFactories () );
	}
	else if ( cur_lex.type == LEX_AUTO_FACTORIES ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunAutoFactories () );
	}
	else if ( cur_lex.type == LEX_MANUFACTURED ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunManufactured () );
	}
	else if ( cur_lex.type == LEX_RESULT_RAW_SOLD ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunResultRawSold () );
	}
	else if ( cur_lex.type == LEX_RESULT_RAW_PRICE ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunResultRawPrice () );
	}
	else if ( cur_lex.type == LEX_RESULT_PROD_BOUGHT ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunResultProdBought () );
	}
	else if ( cur_lex.type == LEX_RESULT_PROD_PRICE ) {
		arg1 ();
		rpn.add_to_list ( new PolizFunResultProdPrice () );
	}
	else 
	{
		throw LexException (
			"Not found function with return.", 
			cur_lex
		);
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

void Parser:: paste_newline ()
{
	rpn.add_to_list ( new PolizString (newline) );
	rpn.add_to_list ( new PolizFunPrint (LEX_STR) );
}


void Parser:: stringelem ()
{
	if ( cur_lex.type == LEX_STR ) {
		int value = cur_lex.value;

		get_lex ();

		char * str = table->string.index (value)->name;
		rpn.add_to_list ( new PolizString (str) );
		rpn.add_to_list ( new PolizFunPrint (LEX_STR) );
	}
	else if ( 
		cur_lex.type == LEX_NUM || 
		cur_lex.type == LEX_ID || 
		cur_lex.type == LEX_ARRAY ||
		look (cur_lex.type , TableLexem:: lex_function)
	)
	{
		D ();	
		rpn.add_to_list ( new PolizFunPrint (LEX_NUM) );
	}
	else {
		throw LexException ("Expected expression or string.",
			cur_lex
		);
	}
}


void Parser:: assign ()
{
	if ( cur_lex.type == LEX_EQ) {
		get_lex ();
		D ();
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

	int place_false = rpn.get_size ();

	create_if_labels ();

	C ();	

	fill_if_labels (place_false);
}


void Parser:: create_if_labels ()
{
	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGoFalse () );
}


void Parser:: fill_if_labels (int place_false)
{
	int place_nop = rpn.get_size ();

	rpn.add_to_list ( new PolizNop () );
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place_nop)),
		place_false
	);
}


void Parser:: whiledo ()
{
	if ( cur_lex.type != LEX_WHILE ) {
		throw LexException ("Must be while. Source code", cur_lex);
	}
	
	int place_compare = rpn.get_size ();

	get_lex ();

	D ();
	
	int place_if_nop = rpn.get_size ();

	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGoFalse () );

	C ();

	fill_while_labels (place_compare, place_if_nop);
}


void Parser:: fill_while_labels (int place_compare, int place_if_nop)
{
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place_compare)) );
	rpn.add_to_list ( new PolizOpGo () );

	int place_nop = rpn.get_size ();

	rpn.add_to_list ( new PolizNop () );
	rpn.add_to_list ( new PolizLabel ( rpn.get_pointer (place_nop) ),
		place_if_nop
	);
}


void Parser:: add_address_id (int value)
{
	int * id = table->ident.index (value)->get_address_value ();
	rpn.add_to_list ( new PolizVarAddress ( id ) );
}


void Parser:: for_loop ()
{
	if ( cur_lex.type != LEX_FOR ) {
		throw LexException ("Must be for. Source code", cur_lex);
	}

	get_lex ();
	lparen ();
	add_address_id (cur_lex.value);
	get_lex ();
	assign ();

	comma ();

	int place_compare = rpn.get_size ();
	D ();
	int place_goto_nop = rpn.get_size ();
	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGoFalse () );
	int place_goto_body = rpn.get_size ();
	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGo () );

	comma ();

	// neg ?

	int place_inc = rpn.get_size ();
	add_address_id (cur_lex.value);
	get_lex ();
	assign ();
	int place_goto_compare = rpn.get_size ();
	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGo () );
	int place_body = rpn.get_size ();
	rparen ();
	C ();
	int place_goto_inc = rpn.get_size ();
	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGo () );

	int place_nop = rpn.get_size ();
	rpn.add_to_list ( new PolizNop () );

	fill_for_labels (place_compare, place_goto_nop, 
		place_goto_body, place_inc, 
		place_goto_compare, place_body, 
		place_goto_inc, place_nop
	);
}


void Parser:: 
fill_for_labels ( 
	int place_compare, int place_goto_nop, 
	int place_goto_body, int place_inc, 
	int place_goto_compare, int place_body, 
	int place_goto_inc, int place_nop
)
{
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place_inc)),
		place_goto_inc	
	);
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place_compare)),
		place_goto_compare
	);
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place_nop)),
		place_goto_nop
	);
	rpn.add_to_list ( new PolizLabel (rpn.get_pointer (place_body)),
		place_goto_body
	);
}


void Parser:: array (bool var)
{
	if ( cur_lex.type != LEX_ARRAY ) {
		throw LexException ("Must be array. Source code", cur_lex);
	}

	int value = cur_lex.value;
	
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

	if ( var == false ) {
		rpn.add_to_list ( new PolizArray (value, table) );
	}
	else {
		rpn.add_to_list ( new PolizVarAddressArray (value, table) );
	}
}


void Parser:: gotolabel ()
{
	if ( cur_lex.type != LEX_GOTO ) {
		throw LexException ("Must be 'goto'. Source code.", cur_lex);
	}
	
	get_lex ();

	int value = cur_lex.value;
	if ( cur_lex.type == LEX_LABEL ) {
		get_lex ();
	}
	else {
		throw LexException ("Error in label of goto", cur_lex);
	}

	labelgoto.put (value, rpn.get_size ());

	create_goto_label ();
}


void Parser:: create_goto_label ()
{
	rpn.add_to_list ( new PolizLabel (0) );
	rpn.add_to_list ( new PolizOpGo () );
}


void Parser:: add_label (const Lex & cur_lex)
{
	int i = label.put ( cur_lex.value, rpn.get_size () );

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
	get_lex ();
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
	if ( type == LEX_EQ ) {
		rpn.add_to_list ( new PolizFunEq () );
	}
	else if ( type == LEX_GREATER ) {
		rpn.add_to_list ( new PolizFunGreater () );
	}
	else if ( type == LEX_LESS ) {
		rpn.add_to_list ( new PolizFunLess () );
	}
	else if ( type == LEX_LEQ ) {
		rpn.add_to_list ( new PolizFunLessEq () );
	}
	else if ( type == LEX_GEQ) {

		rpn.add_to_list ( new PolizFunGreaterEq () );
	}
	else {
		throw ("Must be EQ/GREATER/LESS. Source code");
	}
}


void Parser:: add_switch_E (type_of_lex type)
{

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


void Parser:: check_labels ()
{
	int size_labelgoto = labelgoto.get_size ();	
	int size_label = label.get_size ();	
	int place_label;
	int place_goto;

	printf ("label print\n");
	label.print ();
	printf ("labelgoto print\n");
	labelgoto.print ();
	printf ("end print\n");

	for ( int i = 1; i <= size_labelgoto; ++ i ) {
		bool f_label = false;

		int lblgoto = labelgoto.index (i)->label;
		
		for ( int j = 1; j <= size_label; ++ j ) {
			if ( lblgoto == label.index (j)->label ) {
				f_label = true;
				place_label = label.index (j)->place;
				place_goto = labelgoto.index (i)->place;
				break;
			}
		}

		if ( f_label == false ) {
			throw ("Semantic error. Not found label where goto");
		}

		PolizItem * ptr = rpn.get_pointer (place_label);
		rpn.add_to_list ( new PolizLabel (ptr), place_goto);
	}
}
