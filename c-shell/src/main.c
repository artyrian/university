#include <stdio.h>
#include "lexer.h"


int main (int argc, char ** argv)
{
	ListElem * list_lexems;
	TreeCmd * tree_cmd;

	printf ("Program: c-shell (university). Refactoring. [v.:0.01]\n");

	list_lexems = fill_list ();

	tree_cmd = parse (list_lexems);

	return 0;
}
