#include "player.hpp"

#include <stdio.h>




Player::Player (char *n, int r, int pr, int m, int pl, int apl)
	: nick(n), raw(r), prod(pr), money(m), plants(pl), autoplants(apl)	
{
}




ListPlayer::ListPlayer ()
	: first (0), cnt (0)
{
}

LastElem * ListPlayer::newplayer (Player *pl)
{
	class ListElem *elem = new ListElem;

	elem->p = pl;  
	elem->next = 0;
	
	return elem;
}

LastElem * ListPlayer::findlast ()
{
	class ListElem *cur = first;
	class ListElem *last = first;

	while ( cur != 0 ) {
		last = cur;
			cur = cur->next;
	}

	return last;
}



void ListPlayer::add (Player *pl, ListPlayer *lp)
{
	ListElem *elem = newplayer (pl);
	ListElem *last = findlast ();

	if ( first == 0 ) {
		first = elem;
	} else {
		last->next = elem;
	}
	cnt++;
}

void ListPlayer::search ()
{
}


void ListPlayer::remove ()
{
}


int ListPlayer::getplayercnt ()
{
	return cnt;
}

ListPlayer::Player * parse (char *str)
{
	char *cmd, *nick;
	int raw, prod, money, plant, autoplant;

	raw = prod = money = plant = autoplant = -1; 
	cmd = (char *) malloc (6);
	nick = (char *) malloc (20);

	printf ("Start parse.\n");
	sscanf(str, "%s%s%s%d%d%d%d%d", cmd, cmd,  nick, &raw, &prod, &money, &plant, &autoplant);
	printf ("I parsed:\ncmd[%s]\nnick[%s]\nraw[%d]\nprod[%d]\nmoney[%d]\nplant[%d]\nautoplant[%d].\n",
		cmd, nick, raw, prod, money, plant, autoplant);

	if ( 	(raw == -1) || (prod == -1) || 
		(money == -1) || (plant == -1) ||
		(autoplant == -1) )
	{
		perror ("Syntax error in parse '& INFO'.\n");
		exit (1);
	}

	free (cmd);

	return new Player (nick, raw, prod, money, plant, autoplant);
}

