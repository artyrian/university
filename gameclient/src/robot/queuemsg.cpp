#include "queuemsg.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>


QueueMsg::QueueElem * QueueMsg::create (char *str)
{
	QueueElem *item = new QueueElem;
	item->str = new char [strlen(str) + 1];
	strcpy (item->str, str);
	item->type = str[0]; 
	item->next = 0;

	return item;
}



void QueueMsg::add (char *str)
{
	QueueElem *item = create (str);	

	if ( last == 0 ) {
		last = item;
		first = item;
	} else {
		last->next = item;
		last = item;
	}
	
	cnt++;
}



/* remove first from queue
 */
void QueueMsg::remove ()
{
	QueueElem *next = first->next;
	if ( first == last ) {
		last = next;
	}
//	printf ("Now delete [%s].\n", first->str);
	delete [] first->str;
	delete first;
	first = next;

	cnt--;
}



int QueueMsg::comment (const char c) const
{
	int r = 0;
	if (c != '@') {
		r = 1;
	}
	return r;
}



QueueMsg::QueueMsg (char *ip, int port)
	: ch (ip, port), first (0), last (0), cnt (0)
{
}

char * QueueMsg::gettype ()
{
	strcpy (msg, ch.getmsg ());

	return msg;
}


char * QueueMsg::gettype (int type)
{
	do {
		strcpy (msg, ch.getmsg ());
		if ( (msg[0] != type) && (comment (msg[0]) == 0)) {
//			printf ("Put to queue:[%s].\n", msg);
			add (msg);	
		}
	} while ( msg[0] != type );

	return msg;
}



char * QueueMsg::getmsgq()
{
	strcpy (msgq, first->str);
	remove ();

	return msgq;
}



QueueMsg::~QueueMsg ()
{
	QueueElem *cur = first;	
	while ( first != 0 ) {
		first = cur->next;
		delete [] cur->str;
		delete cur;
		cur = first;
	}
}



void QueueMsg::sendstr (char *str) const
{
	ch.sendstr (str);
}


int QueueMsg::getcount () const 
{
	return cnt;
}


