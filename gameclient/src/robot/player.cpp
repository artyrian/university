#include "player.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Player::Player (char *n, int r, int pr, int m, int pl, int apl, int l_prod)
	: 	nick(n), 
		raw(r),
		prod(pr),
		money(m),
		plants(pl),
		autoplants(apl),
		last_prod (l_prod),
		factive (1)
{
}


/* This operator only for parsed copy!!!
 */
void Player::operator= (Player *p)
{
	raw = p->raw;
	prod = p->prod;
	money = p->money;
	plants = p->plants;
	autoplants = p->autoplants;
	last_prod = prod - last_prod;
}

Player::~Player ()
{
	delete [] nick;
}


ListPlayer::ListPlayer ()
	: first (0), cnt (1)
{
}



ListPlayer::ListElem * ListPlayer:: newplayer (Player *pl)
{
	class ListElem *elem = new ListElem;

	elem->p = pl;  
	elem->next = 0;
	
	return elem;
}

ListPlayer::ListElem * ListPlayer::findlast () const
{
	class ListElem *cur = first;
	class ListElem *last = first;

	while ( cur != 0 ) {
		last = cur;
		cur = cur->next;
	}

	return last;
}



void ListPlayer::add (Player *pl)
{
	ListElem *elem = newplayer (pl); 
	ListElem *last = findlast (); 
	elem->number = ++ cnt;

	if ( first == 0 ) {
		first = elem;
	} else {
		last->next = elem;
	}
}



Player * ListPlayer::find (const char *nick) const
{
	ListElem *cur = first;
	while ( cur != 0 ) {
		if ( strcmp (nick, cur->p->nick) == 0 ) {
			return cur->p;
		}
		cur = cur->next;
	}

	printf ("Not found.\n");
	return 0;
}

Player * ListPlayer::find (int player_num) const
{
	ListElem *cur = first;
	while ( cur != 0 ) {
		if ( player_num ==  cur->number  ) {
			return cur->p;
		}
		cur = cur->next;
	}

	printf ("Not found.\n");
	return 0;
}



Player * ListPlayer::parse (char *str) const
{
	char *trash, *nick;
	int raw, prod, money, plant, autoplant, l_prod;

	raw = prod = money = plant = autoplant = -1; 
	l_prod = 0;
	trash = new char [15];
	nick = new char [20];

	printf ("Start parse.\n");
	sscanf ( str, "%s%s%s%d%d%d%d%d", 
			trash, trash, 
			nick, &raw, &prod, &money, &plant, &autoplant
		);

	if ( 	(raw == -1) || (prod == -1) || 
		(money == -1) || (plant == -1) ||
		(autoplant == -1) )
	{
		perror ("Syntax error in parse '& INFO'.\n");
		exit (1);
	}

	delete [] trash;

	return new Player (nick, raw, prod, money, plant, autoplant, l_prod);
}



ListPlayer::~ListPlayer() 
{
	while ( first != 0 ) {
		struct ListElem *cur = first;
		first = first->next;
		delete cur->p;
		delete cur;
	}
}
