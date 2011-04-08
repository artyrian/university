#include "poliz.hpp"
#include "tables.hpp"

/*
PolizElem:: ~PolizElem ()
{
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

PolizLabel:: PolizLabel ()
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

PolizElem * PolizLabel:: get () const
{
	return value;
}



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


void Poliz:: print ()
{
	PolizElem * cur = first; 

	printf ("Print Poliz List:\n");
	while ( cur != 0 ) {
		cur->lex->print ();	
		cur = cur->next; }
	printf ("\nEnd of Poliz List.\n");
}
