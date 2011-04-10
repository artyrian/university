#include "rpn.hpp"
#include "tables.hpp"

/*
template <class T>
Stack <T>:: Stack ()
{
	max_size = 100;
	top = 0;
}


template <class T>
void Stack <T>:: reset ()
{
	top = 0;
}


template <class T>
void Stack <T>:: push (T i)
{
	if  ( !is_full () ) {
		s [top] = i;
		++ top;
	}
	else {
		throw "Stack is full";
	}
}



template <class T>
T Stack <T>:: pop ()
{
	if ( !is_empty () ) {
		-- top;
		return s [top];
	}
	else {
		throw "Stack is empty";
	}
}


template <class T>
bool Stack <T>:: is_empty ()
{
	return ( top == 0);
}


template <class T>
bool Stack <T>:: is_full ()
{
	return ( top == max_size );
}
*/

////
////
////

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


PolizElem * PolizConst:: clone () const
{
	return new PolizConst ();
}


void PolizConst:: evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const
{
	push (stack, clone ());
	*cur_cmd = (* cur_cmd)->next;
/*
	PolizElem * res = evaluate_fun (stack);
	if ( res ) {
		push (stack, res);
	}
	* cur_cmd = (* cur_cmd)->next;
*/
}


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


/*
PolizOpGo:: PolizOpGo ()
{
}


PolizOpGo:: ~ PolizOpGo ()
{
}


void PolizOpGo:: evaluate ( PolizItem ** stack,
			    PolizItem ** cur_cmd)
const
{
	PolizElem * operand1 = pop (stack);
	PolizLabel * lab = dynamic_cast<PolizLabel *>(operand1);
	if ( !lab ) throw PolizExNoLabel (operand1);
	PolizItem * addr = lab->get ();
	* cur_cmd = addr;
	delete operand1;
}
*/

/*
PolizFunPlus:: PolizFunPlus ()
{
}

PolizFunPlus:: ~PolizFunPlus ()
{
}

PolizFunPlus:: evaluate_fun (PolizItem ** stack) const
{
	PolizElem * operand1 = pop (stack);
	PolizLabel *i1 = dynamic_cast<PolizInt *>(operand1);
	if ( ! i1 ) throw PolizExNotInt (operand1);
}
lizElem *operand2 = Pop(stack);
	PolizLabel *i2 = dynamic_cast<PolizInt*>(operand2);
	if(!i2) throw PolizExNotInt(operand2);
	int res = i1->Get() + i2->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}
*/



////
////
////
/*
Poliz:: Poliz ()
{
	first = 0;
	last = 0;
	size = 0;
}


Poliz:: ~Poliz ()
{
	PolizElem * cur = first;

	while ( cur != 0 ) {
		first = cur;
		cur = cur->next;	
		delete first->lex;
		delete first;
	}
}


Poliz:: PolizElem * 
Poliz:: create_elem (const Lex & l)
{
	PolizElem * t = new PolizElem ;
	t->lex = new Lex (l);
	t->number = size;
	t->next = 0;

	return t;
}


void Poliz:: put_lex (const Lex & l)
{
	printf ("Paste lex:");
	l.print ();
	printf ("\n");

	PolizElem * cur = first;

	if ( first == 0 ) {
		first = create_elem (l);
	}
	else {
		PolizElem * prev;
		while ( cur != 0 ) {
			prev = cur;
			cur = cur->next;
		}
		cur = create_elem (l);	
		prev->next = cur;
	}

	++ size;
}



void Poliz:: put_lex (const Lex & l, int place)
{
	if ( place > size ) {
		throw "POLIZ: indefinite element of array";
	}

	printf ("Paste lex (place):");
	l.print ();
	printf ("\n");

	PolizElem * cur = first;

	int cnt = 0;
	while ( cnt != place ) {
		cur = cur->next;
		++ cnt;
	}

	*(cur->lex) = Lex (l);
}


void Poliz:: blank ()
{
	put_lex ( Lex () ); 
	++ size;
}


int Poliz:: get_size ()
{
	return size;
}
*/

PolizTest:: PolizTest (const Lex & lex)
{
	l = lex;
}


void PolizTest:: print () const
{
	l.print ();
}


PolizItem * PolizList:: create_item (PolizElem * p)
{
	PolizItem * tmp = new PolizItem;
	tmp->p = p;
	tmp->next = 0;

	return tmp;
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



PolizList:: PolizList ()
{
	first = 0;
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
		cur->p->print ();	
		cur = cur->next;
	}

	printf ("\nEnd of PolizList.\n");
}


