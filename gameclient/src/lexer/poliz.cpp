#include "poliz.hpp"

PolizElem:: ~PolizElem ()
{
}


PolizInt (int a)
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
