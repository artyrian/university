#include "executer.hpp"


Executer:: Executer (PolizItem * list)
{
	poliz_list = list;
}


void Executer:: executing ()
{
	while ( poliz_list != 0 ) {
//		poliz_list->p->evaluate ();
		poliz_list = poliz_list->next;
	}
}
