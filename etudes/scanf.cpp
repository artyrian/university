#include <stdio.h>
#include <stdlib.h>

void parse (char *);

int main (int argc, char **argv)
{
	parse (argv[1]);

	return 0;
}



 void parse (char *str) 
 {
	char *cmd, *nick;
	int raw, prod, money, plant, autoplant;

	cmd = (char *) malloc (6);
	nick = (char *) malloc (20);

	sscanf(str, "%s%s%d%d%d%d%d", cmd, nick, &raw, &prod, &money, &plant, &autoplant);

	printf ("I parsed:\ncmd[%s]\nnick[%s]\nraw[%d]\nprod[%d]\nmoney[%d]\nplant[%d]\nautoplant[%d].\n", cmd, nick, raw, prod, money, plant, autoplant);
}

