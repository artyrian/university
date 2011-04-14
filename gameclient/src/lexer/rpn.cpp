#include "rpn.hpp"
#include "tables.hpp"
#include "../exception/exception.hpp"


Stack:: Stack ()
{
	first = 0;
	size = 1;
}


Stack:: ~Stack ()
{
	PolizItem * cur = first;

	while (first != 0) {
		cur = first;
		first = first->next;
		delete cur; 
	}
}


PolizItem * Stack:: create_item (PolizElem * cur_cmd)
{
	return new PolizItem (size ++, cur_cmd, first);
}


void Stack:: push (PolizElem * cur_cmd)
{
	first = create_item (cur_cmd);
}


PolizElem * Stack:: pop ()
{
	PolizItem * cur = first;
	PolizElem * elem = first->p;

	first = first->next;
	delete cur;

	return elem;
}


//----------------------------------------------------------


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
}


PolizElem * PolizElem:: pop (PolizItem ** stack)
{
	return 0;	
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


PolizInt:: PolizInt (int a)
{
	value = a;
}


PolizElem * PolizInt:: clone () const
{
	return new PolizInt (value);
}


int PolizInt:: get () const
{
	return value;
}


void PolizInt:: 
print () const
{
	printf ("! POLIZ_INT ( %d )\t", value);
}


//----------------------------------------------------------

PolizString:: PolizString (char * p) 
{
	value = p;
}

PolizString:: ~PolizString ()
{
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

PolizVarAddress:: PolizVarAddress (int * v)
{
	value = v;
}


PolizElem * PolizVarAddress:: clone () const
{
	return new PolizVarAddress (value);
}


int * PolizVarAddress:: get () const
{
	return value;
}


void PolizVarAddress:: 
print () const
{
	printf ("! POLIZ_VAR_ADDRESS\t");
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
	printf ("! P_LABEL p=%d.\t", reinterpret_cast<int> (value) );
}

//----------------------------------------------------------

PolizOpGo:: PolizOpGo ()
{
}


PolizOpGo:: ~ PolizOpGo ()
{
}


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

PolizOpGoFalse:: PolizOpGoFalse ()
{
}


PolizOpGoFalse:: ~ PolizOpGoFalse ()
{
}


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
		* cur_cmd = addr;
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
	// have index;
	
	int res = table->array [ array ] [ i->get () ].get_value ();
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
	
	int * res = table->array [ array ] [ i->get () ].get_address_value ();
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
	if ( !addr) {
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

	int res = i1->get() ==  i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunEq:: 
print () const
{
	printf ("! POLIZ_EQ\t\t");
}


/*
PolizFunGreater:: PolizFunGreater ()
{
}
*/


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

	int res = i1->get() > i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}

void PolizFunGreater:: 
print () const
{
	printf ("! POLIZ_GREATER\t\t");
}

/*
PolizFunLess:: PolizFunLess ()
{
}
*/


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

	int res = i1->get() < i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunLess:: 
print () const
{
	printf ("! POLIZ_LESS\t\t");
}

/*
PolizFunPlus:: PolizFunPlus ()
{
}
*/


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

	int res = i1->get() + i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunPlus:: 
print () const
{
	printf ("! POLIZ_PLUS\t\t");
}

/*
PolizFunMinus:: PolizFunMinus ()
{
}
*/

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

	int res = i1->get() - i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunMinus:: 
print () const
{
	printf ("! POLIZ_MINUS\t\t");
}

/*
PolizFunOr:: PolizFunOr ()
{
}
*/


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

	int res = i1->get() || i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunOr:: 
print () const
{
	printf ("! POLIZ_OR\t\t");
}

/*
PolizFunMul:: PolizFunMul ()
{
}
*/

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

	int res = i1->get() * i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunMul:: 
print () const
{
	printf ("! POLIZ_MUL\t\t");
}

/*
PolizFunDiv:: PolizFunDiv()
{
}
*/


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

	int res = i1->get() / i2->get();

	delete operand1;
	delete operand2;

	return new PolizInt (res);
}


void PolizFunDiv:: 
print () const
{
	printf ("! POLIZ_DIV\t\t");
}

/*
PolizFunAnd:: PolizFunAnd ()
{
}
*/


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

	int res = i1->get () && i2->get ();

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

	int res = ! i->get ();
	
	delete operand;

	return new PolizInt (res);
}


void PolizFunNeg:: 
print () const
{
	printf ("! POLIZ_NEG\t\t");
}

//----------------------------------------------------------

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

	printf ("Here call buy (%d, %d).\n", i2->get (), i1->get ());
	
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

	printf ("Here call sell (%d, %d).\n", i2->get (), i1->get ());
	
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

PolizElem * PolizFunProd:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast<PolizInt *>(operand1);
	if ( ! i1 ) {
		throw PolizExceptionNotInt (operand1);
	}

	printf ("Here call prod (%d).\n", i1->get ());
	
	delete operand1;

	return 0;
}


void PolizFunProd:: 
print () const
{
	printf ("! POLIZ_PROD\t\t");
}

//----------------------------------------------------------

PolizElem * PolizFunBuild:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call build ().\n");
	
	return 0;
}


void PolizFunBuild:: 
print () const
{
	printf ("! POLIZ_BUILD\t\t");
}

//----------------------------------------------------------

PolizElem * PolizFunTurn:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call turn ().\n");
	
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
	if ( type == LEX_STR ) {
		PolizInt * i1 = dynamic_cast 
			<PolizInt *> (operand1);
		printf ("call print (%d).\n", i1->get ());
		delete operand1;
	}
	else if ( type == LEX_NUM ) {
		PolizString * i1 = dynamic_cast
			<PolizString *> (operand1);
		printf ("call print (%s).\n", i1->get ());
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

PolizElem * PolizFunCurMonth:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call cur_month ()\n");
	
	return 0;
}


void PolizFunCurMonth:: 
print () const
{
	printf ("! POLIZ_CUR_MONTH\t\t");
}

//----------------------------------------------------------

PolizElem * PolizFunPlayers:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call players ()\n");
	
	return 0;
}


void PolizFunPlayers:: 
print () const
{
	printf ("! POLIZ_PLAYERS\t\t");
}

//----------------------------------------------------------

PolizElem * PolizFunActivePlayers:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call acitve_players ()\n");
	
	return 0;
}


void PolizFunActivePlayers:: 
print () const
{
	printf ("! POLIZ_ACTIVE_PLAYERS\t");
}

PolizElem * PolizFunSupply:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call supply ()\n");
	
	return 0;
}


void PolizFunSupply:: 
print () const
{
	printf ("! POLIZ_SUPLLY\t");
}

PolizElem * PolizFunRawPrice:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call raw_price ()\n");
	
	return 0;
}


void PolizFunRawPrice:: 
print () const
{
	printf ("! POLIZ_RAW_PRICE\t");
}

PolizElem * PolizFunDemand:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call demand ()\n");
	
	return 0;
}


void PolizFunDemand:: 
print () const
{
	printf ("! POLIZ_DEMAND\t");
}

PolizElem * PolizFunProductionPrice:: 
evaluate_fun (PolizItem ** stack) const
{
	printf ("Here call production_price ()\n");
	
	return 0;
}


void PolizFunProductionPrice:: 
print () const
{
	printf ("! POLIZ_PRODUCTION_PRICE\t");
}


PolizElem * PolizFunMoney:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i1 = dynamic_cast <PolizInt *> (operand1);
	if ( !i1 ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call money (%d)", i1->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunMoney:: 
print () const
{
	printf ("! POLIZ_MONEY\t\t");
}


PolizElem * PolizFunRaw:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call raw (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunRaw:: 
print () const
{
	printf ("! POLIZ_RAW\t\t");
}


PolizElem * PolizFunProduction:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call production (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunProduction:: 
print () const
{
	printf ("! POLIZ_PRODUCTION\t\t");
}


PolizElem * PolizFunFactories:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call factories (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunFactories:: 
print () const
{
	printf ("! POLIZ_FACTORIES\t\t");
}


PolizElem * PolizFunAutoFactories:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call auto_factories (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunAutoFactories:: 
print () const
{
	printf ("! POLIZ_AUTO_FACTORIES\t\t");
}


PolizElem * PolizFunManufactured:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call manufactured (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunManufactured:: 
print () const
{
	printf ("! POLIZ_MANUFACTURED\t\t");
}


PolizElem * PolizFunResultRawSold:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call result_raw_sold (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultRawSold:: 
print () const
{
	printf ("! POLIZ_RESULT_RAW_SOLD\t\t");
}


PolizElem * PolizFunResultRawPrice:: 

evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call result_raw_PRICE (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultRawPrice::
print () const
{
	printf ("! POLIZ_RESULT_RAW_PRICE\t");
}


PolizElem * PolizFunResultProdBought:: 

evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call result_prod_bought (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultProdBought:: 
print () const
{
	printf ("! POLIZ_RESULT_PROD_BOUGHT\t");

}


PolizElem * PolizFunResultProdPrice:: 
evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizInt * i = dynamic_cast <PolizInt *> (operand1);
	if ( !i ) { 
		throw PolizExceptionNotInt (operand1);
	}
	
	int res = 0;
	printf (" Here call result_prod_price (%d)", i->get ());

	delete operand1;

	return new PolizInt (res);
}


void PolizFunResultProdPrice:: 
print () const
{
	printf ("! POLIZ_RESULT_PROD_PRICE\t");

}


//----------------------------------------------------------

PolizTest:: PolizTest (const Lex & lex)
{
	l = lex;
}


void PolizTest:: 
evaluate (PolizItem ** stack, PolizItem ** cur_cmd) const 
{
}


void PolizTest:: 
print () const
{
	l.print ();
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
	
	printf ("Print PolizList:\n");

	while ( cur != 0 ) {
		printf ("%d:\t", cur->number);
		cur->p->print ();	
		printf ("obj_ptr: %d", reinterpret_cast<int> (cur));
		cur = cur->next;
		printf ("\n");
	}

	printf ("\nEnd of PolizList.\n");
}

//----------------------------------------------------------
