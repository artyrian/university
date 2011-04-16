#include "executer.hpp"


Executer:: Executer (PolizItem * list)
{
	poliz_list = list;
	stack = 0;
}


Executer:: ~Executer ()
{
}


void Executer:: executing ()
{
	printf ("\nStart execute program:\n");
	while ( poliz_list != 0 ) {
		current = poliz_list;
		poliz_list->p->evaluate (&stack, &current);
		poliz_list = current;
	}
	printf ("End of program.\n");
}


void Executer:: print_stack () const 
{
	PolizItem * cur = stack; 

	printf ("print stack:\n");
	while ( cur != 0 ) {
		cur->p->print ();
		cur = cur->next;
	}

	printf ("end of stack.\n");

}
