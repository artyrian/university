#include "rpn.hpp"
#include "../exception/exception.hpp"


PolizItem:: PolizItem (int n, PolizElem * ptr, PolizItem * p_next)
	: number (n), p (ptr), next (p_next)
{
}

//----------------------------------------------------------

PolizElem:: ~PolizElem ()
{
}

void PolizElem:: push (PolizItem ** stack, PolizElem * cur_cmd)
{
 	PolizItem * cur = new PolizItem ( );
	cur->next = (*stack);
	cur->p = cur_cmd;
	(*stack) = cur;
}


PolizElem * PolizElem:: pop (PolizItem ** stack)
{
	PolizItem * cur = (*stack);
	PolizElem * elem = (*stack)->p;
	(*stack) = (*stack)->next;

	delete cur;

	return elem;
}


void PolizElem:: 
print () const
{
	printf ("! Unkown POLIZ print.\t");
}

//----------------------------------------------------------

void PolizNop:: 
evaluate (PolizItem ** stack, PolizItem ** cur_cmd) const
{
	*cur_cmd = (* cur_cmd)->next;
}


void PolizNop:: 
print () const
{
	printf ("! POLIZ_NOP\t\t");
}

//----------------------------------------------------------

PolizElem * PolizConst:: clone () const
{
	return new PolizConst ();
}


void PolizConst:: evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const
{
	push (stack, clone ());
	*cur_cmd = (* cur_cmd)->next;
}


//----------------------------------------------------------


PolizInt:: PolizInt (long long int a)
{
	value = a;
}


PolizElem * PolizInt:: clone () const
{
	return new PolizInt (value);
}


long long int PolizInt:: get () const
{
	return value;
}


void PolizInt:: 
print () const
{
	printf ("! POLIZ_INT\t\t");
}

//----------------------------------------------------------


PolizVarInt:: PolizVarInt (long long int * a)
{
	value = a;
}


PolizElem * PolizVarInt:: clone () const
{
	return new PolizInt (*value);
}


long long int PolizVarInt:: get () const
{
	return (*value);
}


void PolizVarInt:: 
print () const
{
	printf ("! POLIZ_VAR_INT\t\t");
}




//----------------------------------------------------------

PolizString:: PolizString (char * p) 
{
	value = p;
}


PolizElem * PolizString:: clone () const
{
	return new PolizString (value);
}

char * PolizString:: get () const
{
	return value;
}

void PolizString:: 
print () const
{
	printf ("! POLIZ_STR\t\t");
}

//----------------------------------------------------------

PolizVarAddress:: PolizVarAddress (long long int * v)
{
	value = v;
}


PolizElem * PolizVarAddress:: clone () const
{
	return new PolizVarAddress (value);
}


long long int * PolizVarAddress:: get () const
{
	return value;
}


void PolizVarAddress:: 
print () const
{
	printf ("! POLIZ_VAR_ADDR \t");
}

//----------------------------------------------------------

PolizLabel:: PolizLabel (PolizItem * a)
{
	value = a;
}


PolizLabel:: ~PolizLabel ()
{
}


PolizElem * PolizLabel:: clone () const
{
	return new PolizLabel (value);
}


PolizItem * PolizLabel:: get () const
{
	return value;
}

void PolizLabel:: 
print () const
{
	printf ("! P_LABEL p=%x.\t", reinterpret_cast<int> (value) );
}

//----------------------------------------------------------

void PolizOpGo :: 
evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const
{
	PolizElem * operand1 = pop (stack);

	PolizLabel * lab = dynamic_cast<PolizLabel *>(operand1);

	if ( !lab ) {
		throw PolizExceptionNotLabel (operand1);
	}

	PolizItem * addr = lab->get ();

	* cur_cmd = addr;

	delete operand1;
}

void PolizOpGo:: 
print () const
{
	printf ("! POLIZ_OP_GO\t\t");
}

//----------------------------------------------------------

void PolizOpGoFalse:: 
evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const
{
	PolizElem * operand1 = pop (stack);
	PolizLabel * lab = dynamic_cast<PolizLabel *>(operand1);
	if ( !lab ) {
		throw PolizExceptionNotLabel (operand1);
	}

	PolizElem * operand2 = pop (stack);
	PolizInt * i = dynamic_cast<PolizInt *>(operand2);
	if ( !i ) {
		throw PolizExceptionNotInt (operand2);
	}

	PolizItem * addr = lab->get ();

	if ( i->get () == 0 ) {
		(*cur_cmd) = addr;
	}
	else {
		(*cur_cmd) = (*cur_cmd)->next;
	}
	

	delete operand1;
	delete operand2;
}


void PolizOpGoFalse:: 
print () const
{
	printf ("! P_OP_GO_FALSE\t\t");
}
//----------------------------------------------------------

void PolizFunction:: 
evaluate (PolizItem ** stack, PolizItem ** cur_cmd) const
{
	PolizElem * res = evaluate_fun (stack);

	if ( res != 0 ) {
		push (stack, res);
	}

	(* cur_cmd) = (* cur_cmd)->next;
}

PolizFunction:: ~PolizFunction ()
{
}

//----------------------------------------------------------

PolizArray:: PolizArray (int op, TableLexem * p_table)
{
	array = op;
	table = p_table;
}


PolizElem * PolizArray:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) {
		throw PolizExceptionNotInt (operand1);
	}
	// have address index;

	char str [20];
	sprintf (str, "%lli", i->get ());
	int k = table->array. index (array)-> put ( str );
	// write index.
	
	long long int res = table-> array.index ( array )->index ( k ) -> value;
	// get value elem (i) of array (=value).

	delete operand1;

	return new PolizInt (res);
}


void PolizArray:: 
print () const
{
	printf ("! POLIZ_ARRAY\t\t");
}

//----------------------------------------------------------

PolizVarAddressArray:: PolizVarAddressArray (int arr, TableLexem * p_table)
{
	array = arr;
	table = p_table;
}


PolizElem * PolizVarAddressArray:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) {
		throw PolizExceptionNotInt (operand1);
	}
	// have index;
	
	char str [20];
	sprintf (str, "%lli", i->get ());
	int k = table->array. index ( array ) ->put (str);

	long long int * res = table->array.index (array) ->index (k)->get_address_value ();
	// get value elem (i) of array (=value).

	delete operand1;

	return new PolizVarAddress (res);
}


void PolizVarAddressArray:: 
print () const
{
	printf ("! P_VAR_ADDRESS_ARRAY\t");
}




//----------------------------------------------------------

PolizElem * PolizAssign:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) {
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);
	PolizVarAddress * addr = dynamic_cast <PolizVarAddress *> (operand2);
	if ( !addr ) {
		throw PolizExceptionNotAddress (operand2);
	}

	*( addr->get () ) = i->get ();

	delete operand1;
	delete operand2;

	return 0;
}

void PolizAssign:: 
print () const
{
	printf ("! POLIZ_ASSIGN\t\t");
}

//----------------------------------------------------------

PolizElem * PolizFunEq:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);
	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);
	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);
	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() ==  i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunEq:: 
print () const
{
	printf ("! POLIZ_EQ\t\t");
}


PolizElem * PolizFunGreater:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() > i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}

void PolizFunGreater:: 
print () const
{
	printf ("! POLIZ_GREATER\t\t");
}


PolizElem * PolizFunLess:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() < i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunLess:: 
print () const
{
	printf ("! POLIZ_LESS\t\t");
}



PolizElem * PolizFunGreaterEq:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() >= i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}

void PolizFunGreaterEq:: 
print () const
{
	printf ("! POLIZ_GEQ\t\t");
}


PolizElem * PolizFunLessEq:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() <= i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunLessEq:: 
print () const
{
	printf ("! POLIZ_LEQ\t\t");
}


PolizElem * PolizFunLessGreater:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() != i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunLessGreater:: 
print () const
{
	printf ("! POLIZ_LG\t\t");
}





PolizElem * PolizFunPlus:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() + i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunPlus:: 
print () const
{
	printf ("! POLIZ_PLUS\t\t");
}


PolizElem * PolizFunMinus:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() - i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunMinus:: 
print () const
{
	printf ("! POLIZ_MINUS\t\t");
}


PolizElem * PolizFunOr:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() || i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunOr:: 
print () const
{
	printf ("! POLIZ_OR\t\t");
}


PolizElem * PolizFunMul:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() * i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunMul:: 
print () const
{
	printf ("! POLIZ_MUL\t\t");
}



PolizElem * PolizFunDiv:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get() / i1->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunDiv:: 
print () const
{
	printf ("! POLIZ_DIV\t\t");
}



PolizElem * PolizFunAnd:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);

	if ( ! i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);

	PolizInt * i2 = dynamic_cast <PolizInt*> (operand2);

	if ( !i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	long long int res = i2->get () && i1->get ();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunAnd:: 
print () const
{
	printf ("! POLIZ_AND\t\t");
}



PolizElem * PolizFunNeg:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand = pop (stack);
	PolizInt * i = dynamic_cast<PolizInt *>(operand);

	if ( ! i ) {
		throw PolizExceptionNotInt (operand);
	}

	long long int res = ! i->get ();
	
	delete operand;

	return new PolizInt (res);
}


void PolizFunNeg:: 
print () const
{
	printf ("! POLIZ_NEG\t\t");
}

//----------------------------------------------------------

PolizFunBuy:: PolizFunBuy (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunBuy:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);
	if ( ! i1 ) {
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);
	PolizInt * i2 = dynamic_cast<PolizInt *>(operand2);
	if ( ! i2 ) {
		throw PolizExceptionNotInt (operand2);
	}


	robot->buy (i2->get (), i1->get ());
	
	delete operand1;
	delete operand2;

	return 0;
}


void PolizFunBuy:: 
print () const
{
	printf ("! POLIZ_BUY\t\t");
}

//----------------------------------------------------------

PolizFunSell:: PolizFunSell (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunSell:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);
	if ( ! i1 ) {
		throw PolizExceptionNotInt (operand1);
	}

	PolizElem * operand2 = pop (stack);
	PolizInt * i2 = dynamic_cast<PolizInt *>(operand2);
	if ( ! i2 ) {
		throw PolizExceptionNotInt (operand2);
	}

	robot->sell (i2->get (), i1->get ()); 
	
	delete operand1;
	delete operand2;

	return 0;
}


void PolizFunSell:: 
print () const
{
	printf ("! POLIZ_SELL\t\t");
}

//----------------------------------------------------------

PolizFunProd:: PolizFunProd (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunProd:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);
	if ( ! i1 ) {
		throw PolizExceptionNotInt (operand1);
	}

	robot->prod (i1->get ());
	
	delete operand1;

	return 0;
}


void PolizFunProd:: 
print () const
{
	printf ("! POLIZ_PROD\t\t");
}

//----------------------------------------------------------

PolizFunBuild:: PolizFunBuild (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunBuild:: 
evaluate_fun (PolizItem ** stack) const
{
	robot->build ();
	
	return 0;
}


void PolizFunBuild:: 
print () const
{
	printf ("! POLIZ_BUILD\t\t");
}

//----------------------------------------------------------

PolizFunTurn:: PolizFunTurn (Game * robot_ptr)
{
	robot = robot_ptr;
}



PolizElem * PolizFunTurn:: 
evaluate_fun (PolizItem ** stack) const
{
	robot->turn ();
	
	return 0;
}


void PolizFunTurn:: 
print () const
{
	printf ("! POLIZ_TURN\t\t");
}

//----------------------------------------------------------

PolizFunPrint:: PolizFunPrint (type_of_lex p_type)
{
	type = p_type;
}


PolizElem * PolizFunPrint:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	if ( type == LEX_NUM ) {
		PolizInt * i1 = dynamic_cast 
			<PolizInt *> (operand1);
		printf ("%lli", i1->get ());

		delete operand1;
	}
	else if ( type == LEX_STR ) {
		PolizString * i1 = dynamic_cast
			<PolizString *> (operand1);
		printf ("%s", i1->get ());
		delete operand1;
	}
	else {
		throw ("Error in choose parameters print");
	}

	return 0;
}


void PolizFunPrint:: 
print () const
{
	printf ("! POLIZ_PRINT\t\t");
}

//----------------------------------------------------------

PolizFunMyId:: PolizFunMyId (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunMyId:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt ( robot->convert_to_int (robot->my_id()) );
}


void PolizFunMyId:: 
print () const
{
	printf ("! POLIZ_MY_ID\t\t");
}

//----------------------------------------------------------


PolizFunCurMonth:: PolizFunCurMonth (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunCurMonth:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt ( robot->cur_month () );
}


void PolizFunCurMonth:: 
print () const
{
	printf ("! POLIZ_CUR_MONTH\t\t");
}

//----------------------------------------------------------

PolizFunPlayers:: PolizFunPlayers (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunPlayers:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt (robot->players ());
}


void PolizFunPlayers:: 
print () const
{
	printf ("! POLIZ_PLAYERS\t\t");
}

//----------------------------------------------------------

PolizFunActivePlayers:: PolizFunActivePlayers (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunActivePlayers:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt (robot->active_players ());
}


void PolizFunActivePlayers:: 
print () const
{
	printf ("! POLIZ_ACTIVE_PLAYERS\t");
}

PolizFunSupply:: PolizFunSupply (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunSupply:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt (robot->supply ());
}


void PolizFunSupply:: 
print () const
{
	printf ("! POLIZ_SUPLLY\t");
}


PolizFunRawPrice:: PolizFunRawPrice (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunRawPrice:: 
evaluate_fun (PolizItem ** stack) const
{
	int res = robot->raw_price ();
	return new PolizInt (res);
}


void PolizFunRawPrice:: 
print () const
{
	printf ("! POLIZ_RAW_PRICE\t");
}

PolizFunDemand:: PolizFunDemand (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunDemand:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt (robot->demand ());
}


void PolizFunDemand:: 
print () const
{
	printf ("! POLIZ_DEMAND\t");
}


PolizFunProductionPrice:: PolizFunProductionPrice (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunProductionPrice:: 
evaluate_fun (PolizItem ** stack) const
{
	return new PolizInt (robot->production_price ());
}


void PolizFunProductionPrice:: 
print () const
{
	printf ("! POLIZ_PRODUCTION_PRICE\t");
}


PolizFunMoney:: PolizFunMoney (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunMoney:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->money ( 
		robot->convert_to_char ( i1->get() ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunMoney:: 
print () const
{
	printf ("! POLIZ_MONEY\t\t");
}


PolizFunRaw:: PolizFunRaw (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunRaw:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->raw ( 
		robot->convert_to_char ( i1->get() ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunRaw:: 
print () const
{
	printf ("! POLIZ_RAW\t\t");
}


PolizFunProduction:: PolizFunProduction (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunProduction:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->production ( 
			robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunProduction:: 
print () const
{
	printf ("! POLIZ_PRODUCTION\t\t");
}


PolizFunFactories:: PolizFunFactories (Game * robot_ptr)
{
	robot = robot_ptr;
}




PolizElem * PolizFunFactories:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->factories ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunFactories:: 
print () const
{
	printf ("! POLIZ_FACTORIES\t\t");
}


PolizFunAutoFactories:: PolizFunAutoFactories (Game * robot_ptr)
{
	robot = robot_ptr;
}




PolizElem * PolizFunAutoFactories:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->auto_factories ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunAutoFactories:: 
print () const
{
	printf ("! POLIZ_AUTO_FACTORIES\t\t");
}


PolizFunManufactured:: PolizFunManufactured (Game * robot_ptr)
{
	robot = robot_ptr;
}




PolizElem * PolizFunManufactured:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->manufactured ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunManufactured:: 
print () const
{
	printf ("! POLIZ_MANUFACTURED\t\t");
}


PolizFunResultRawSold:: PolizFunResultRawSold (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunResultRawSold:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->result_raw_sold ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultRawSold:: 
print () const
{
	printf ("! POLIZ_RESULT_RAW_SOLD\t\t");
}


PolizFunResultRawPrice:: PolizFunResultRawPrice (Game * robot_ptr)
{
	robot = robot_ptr;
}




PolizElem * PolizFunResultRawPrice:: 

evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->result_raw_price ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultRawPrice::
print () const
{
	printf ("! POLIZ_RESULT_RAW_PRICE\t");
}


PolizFunResultProdBought:: PolizFunResultProdBought (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunResultProdBought:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->result_prod_bought ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultProdBought:: 
print () const
{
	printf ("! POLIZ_RESULT_PROD_BOUGHT\t");

}


PolizFunResultProdPrice:: PolizFunResultProdPrice (Game * robot_ptr)
{
	robot = robot_ptr;
}


PolizElem * PolizFunResultProdPrice:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	long long int res = robot->result_prod_price ( 
		robot->convert_to_char ( i1->get () ) );

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultProdPrice:: 
print () const
{
	printf ("! POLIZ_RESULT_PROD_PRICE\t");

}

//----------------------------------------------------------

PolizItem * PolizList:: create_item (PolizElem * cur_cmd)
{
	return new PolizItem (size ++, cur_cmd, 0);
}



void PolizList:: add_to_list (PolizElem * p)
{
	PolizItem * cur = first;

	if ( first == 0 ) {
		first = create_item (p); 
	}
	else {
		PolizItem * prev;
		while ( cur != 0 ) {
			prev = cur;
			cur = cur->next;
		}
		cur = create_item (p);	
		prev->next = cur;
	}
}


int PolizList:: get_size () const
{
	return size;
}


PolizItem * PolizList:: get_pointer (int place) const
{
	PolizItem * cur = first;

	int cnt = 1;
	while ( cnt != place ) {
		cur = cur->next;
		++ cnt;
		if ( cur == 0 ) {
			throw ("Wrong place. Next pointer of PolizList is 0.");
		}
	}
	
	return cur;
}


void PolizList:: add_to_list (PolizElem * p, int place)
{
	PolizItem * cur = first;

	int cnt = 1;
	while ( cnt != place ) {
		cur = cur->next;
		++ cnt;
		if ( cur == 0 ) {
			throw ("Error add to place. Next pointer of PolizList is 0.");
		}
	}

	delete cur->p;
	cur->p = p;
}


PolizList:: PolizList ()
{
	first = 0;
	size = 1;
}


PolizList:: ~PolizList ()
{
	PolizItem * cur = first;

	while ( cur != 0 ) {
		first = cur;
		cur = cur->next;	
		delete first;
	}
}


void PolizList:: 
print () const
{
	PolizItem * cur = first;
	
	printf ("\nPrint PolizList:\n");

	while ( cur != 0 ) {
		printf ("%d:\t", cur->number);
		cur->p->print ();	
		printf ("obj_ptr: %x", reinterpret_cast<int> (cur));
		cur = cur->next;
		printf ("\n");
	}

	printf ("End of PolizList.\n");
}

//----------------------------------------------------------
