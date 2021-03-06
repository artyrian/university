#include "player.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int month_raw_cost  = 300;
const int month_prod_cost = 500;
const int month_plants_cost = 1000;

Player:: Player (char *n, int r, int pr, 
	int m, int pl, int apl, int l_prod
)
{
	nick = n; 
	raw = r;
	prod = pr;
	money = m;
	plants = pl;
	autoplants = apl;
	last_prod = l_prod;
}


/* This operator only for parsed copy!!!
&raw, &prod, &money, &plant, &autoplant
 */
Player * Player:: copy (const Player *p)
{
	raw = p->raw;
	prod = p->prod;
	money = p->money;
	plants = p->plants;
	autoplants = p->autoplants;
	last_prod = prod - last_prod;

	return (this);
}

void Player:: print () const
{
	printf ("number:%d,money:%d, raw:%d;\n",
		number, money, raw);
}

Player:: ~Player ()
{
	delete [] nick;
}


ListPlayer:: ListPlayer ()
	: first (0), cnt (0)
{
}



ListPlayer:: ListElem * 
ListPlayer:: newplayer (Player * player)
{
	class ListElem *elem = new ListElem;

	elem->p = player;  
	elem->p->number = ++ cnt;
	elem->next = 0;
	
	return elem;
}

ListPlayer:: ListElem * 
ListPlayer:: findlast () const
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

	if ( first == 0 ) {
		first = elem;
	} else {
		last->next = elem;
	}
}



Player * ListPlayer:: find (const char *nick) const
{
	ListElem *cur = first;
	while ( cur != 0 ) {
		if ( strcmp (nick, cur->p->nick) == 0 ) {
			return cur->p;
		}
		cur = cur->next;
	}

	printf ("Not found player with nick [%s].\n",
		nick);

	return 0;
}

Player * ListPlayer:: find (int player_num) const
{
	ListElem *cur = first;
	while ( cur != 0 ) {
		if ( player_num ==  cur->p->number  ) {
			return cur->p;
		}
		cur = cur->next;
	}

	printf ("Not found player with number [%d].\n",
		player_num);

	return 0;
}



Player * ListPlayer:: parse (char *str) const
{
	char * trash, * nick;
	int raw, prod, money, plant, autoplant, l_prod;

	raw = prod = money = plant = autoplant = -1; 
	l_prod = 0;
	trash = new char [15];
	nick = new char [20];

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

void ListPlayer:: disable_all ()
{
	ListElem * cur = first;

	while ( cur != 0 ) {
		cur->p->factive = false;
		cur = cur -> next;
	}
}


void ListPlayer:: enable (Player * player)
{
	player->factive = true;
}

void ListPlayer:: month_expense ()
{
	ListElem * cur = first;

	while ( cur != 0 ) {
		cur->p->money -= cur->p->raw * month_raw_cost + 
			cur->p->prod * month_prod_cost + 
			cur->p->plants *  month_plants_cost;
		cur = cur -> next;
	}

}

void ListPlayer:: print () const
{
	ListElem * cur = first;

	printf ("Here print list players:\n");

	while ( cur != 0 ) {
		cur->p->print ();
		cur = cur -> next;
	}

	printf ("End of print list players:\n");
}


ListPlayer:: ~ListPlayer() 
{
	while ( first != 0 ) {
		struct ListElem *cur = first;
		first = first->next;
		delete cur->p;
		delete cur;
	}
}
