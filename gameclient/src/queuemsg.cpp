#include "queuemsg.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>


QueueMsg::QueueElem * QueueMsg::create (char *str)
{
	QueueElem *item = new QueueElem;
	item->str = new char [strlen(str)];
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



void QueueMsg::remove ()
{
	QueueElem *next = first->next;
	if ( first == last ) {
		last = next;
	}
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



char * QueueMsg::gettype (int type)
{
	msg = new char [80];	
	msg[0] = '\0';

	do {
		msg = ch.getmsg ();
		if ( (msg[0] != type) && (comment (msg[0]) == 0)) {
			printf ("Put to queue:[%s].\n", msg);
			add (msg);	
		}
	} while ( msg[0] != type );

	printf ("Return with type [%c]:[%s].\n", type, msg);
	return msg;
}



char * QueueMsg::readqueue ()
{
	msgq = new char [strlen(first->str)];
	strcpy (msgq, first->str);
	remove ();

	return msgq;
}



QueueMsg::~QueueMsg ()
{
	QueueElem *cur = first;	
	while ( cur != 0 ) {
		delete [] cur->str;
		cur = first->next;
		delete first;
	}
	delete [] msg;
	delete [] msgq;
}



void QueueMsg::sendstr (char *str) const
{
	ch.sendstr (str);
}


int QueueMsg::getcount () const 
{
	return cnt;
}

