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

void PolizElem:: print () const
{
	printf ("! Unkown POLIZ print.");
}

//----------------------------------------------------------

void PolizNop:: 
evaluate (PolizItem ** stack, PolizItem ** cur_cmd) const
{
}


void PolizNop:: print () const
{
	printf ("! POLIZ_NOP");
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


PolizInt:: ~PolizInt ()
{
}


PolizElem * PolizInt:: clone () const
{
	return new PolizInt (value);
}


int PolizInt:: get () const
{
	return value;
}

void PolizInt:: print () const
{
	printf ("! POLIZ_INT (v. = %d)", value);
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

/*
PolizFunEq:: PolizFunEq ()
{
}
*/

PolizElem * PolizFunEq:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunEq:: print () const
{
	printf ("! POLIZ_EQ");
}


/*
PolizFunGreater:: PolizFunGreater ()
{
}
*/


PolizElem * PolizFunGreater:: evaluate_fun (PolizItem ** stack) const
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

void PolizFunGreater:: print () const
{
	printf ("! POLIZ_GREATER");
}

/*
PolizFunLess:: PolizFunLess ()
{
}
*/


PolizElem * PolizFunLess:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunLess:: print () const
{
	printf ("! POLIZ_LESS");
}

/*
PolizFunPlus:: PolizFunPlus ()
{
}
*/


PolizElem * PolizFunPlus:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunPlus:: print () const
{
	printf ("! POLIZ_PLUS");
}

/*
PolizFunMinus:: PolizFunMinus ()
{
}
*/

PolizElem * PolizFunMinus:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunMinus:: print () const
{
	printf ("! POLIZ_MINUS");
}

/*
PolizFunOr:: PolizFunOr ()
{
}
*/


PolizElem * PolizFunOr:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunOr:: print () const
{
	printf ("! POLIZ_OR");
}

/*
PolizFunMul:: PolizFunMul ()
{
}
*/

PolizElem * PolizFunMul:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunMul:: print () const
{
	printf ("! POLIZ_MUL");
}

/*
PolizFunDiv:: PolizFunDiv()
{
}
*/


PolizElem * PolizFunDiv:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunDiv:: print () const
{
	printf ("! POLIZ_DIV");
}

/*
PolizFunAnd:: PolizFunAnd ()
{
}
*/


PolizElem * PolizFunAnd:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunAnd:: print () const
{
	printf ("! POLIZ_AND");
}



PolizElem * PolizFunNeg:: evaluate_fun (PolizItem ** stack) const
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


void PolizFunNeg:: print () const
{
	printf ("! POLIZ_NEG");
}

//----------------------------------------------------------
////
////
////
//----------------------------------------------------------

PolizTest:: PolizTest (const Lex & lex)
{
	l = lex;
}


void PolizTest:: 
evaluate (PolizItem ** stack, PolizItem ** cur_cmd) const 
{
}


void PolizTest:: print () const
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


int PolizList:: get_size ()
{
	return size;
}


void PolizList:: add_to_list (PolizElem * p, int place)
{
	PolizItem * cur = first;

	while ( cur->number != place ) {
		cur = cur->next;
	}

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


void PolizList:: print ()
{
	PolizItem * cur = first;
	
	printf ("Print PolizList:\n");

	while ( cur != 0 ) {
		printf ("%d: ", cur->number);
		cur->p->print ();	
		cur = cur->next;
		printf ("\n");
	}

	printf ("\nEnd of PolizList.\n");
}

//----------------------------------------------------------
