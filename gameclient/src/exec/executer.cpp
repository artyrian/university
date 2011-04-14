#include "executer.hpp"


Executer:: Executer (PolizItem * list)
{
	poliz_list = list;
}


Executer:: ~Executer ()
{
}


void Executer:: executing ()
{
	while ( poliz_list != 0 ) {
		current = poliz_list;
		poliz_list->p->print ();
		printf ("\n");
		poliz_list->p->evaluate (&stack, &current);
		poliz_list = current;
	}
}
