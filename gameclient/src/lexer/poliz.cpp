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
	last = 0;
	count = 0;
	size = 0;
	last = create_elem ( Lex (0, POLIZ_NOP));
}


Poliz:: ~Poliz ()
{
	PolizElem * cur = last;

	while ( cur != 0 ) {
		last = cur;
		cur = cur->next;	
		delete last->lex;
		delete last;
	}
}


Poliz:: PolizElem * 
Poliz:: create_elem (const Lex & l)
{
	PolizElem * t = new PolizElem ;
	t->lex = new Lex (l);
	t->number = count;
	t->next = last;

	++ count;

	return t;
}


void Poliz:: put_lex (const Lex & l)
{
	last = create_elem (l);

	++ size;
}



void Poliz:: put_lex (const Lex & l, int place)
{
	if ( place > size ) {
		throw "POLIZ: indefinite element of array";
	}

	PolizElem * cur = last;

	while ( size != place ) {
		cur = cur->next;
	}
/*
	PolizElem * t = new PolizElem ;
	t->lex = new Lex (l);
	t->number = count;
	t->next = last;
III!:!!:!JLKJF:LKDJF:O LIO J:OHA


*/
	cur->lex = Lex (l);
}


void Poliz:: blank ()
{
	++ size;
}


int Poliz:: get_size ()
{
	return size;
}


void Poliz:: print ()
{
	PolizElem * cur = first; 

	int cnt = 1;
	printf ("Print Poliz List:\n");
	while ( cur != 0 ) {
		if ( cnt != cur->lex.strnum ) {
			printf ("\n");
			cnt = cur->lex.strnum;
		}
		cur->lex.print ();	
		cur = cur->next; }
	printf ("\nEnd of Poliz List.\n");
}
