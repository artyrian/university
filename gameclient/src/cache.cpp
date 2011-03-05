#include "cache.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>


QueueMsg::Cache::Cache (char *ip, int port) 
	: link (ip, port), cnt(0)
{
	sd = link.connecting ();
	printf ("Now connecting to sd[%d].\n", sd);
	msg[0] = '\0';
	buf[0] = '\0';
	ext[0] = '\0';
}



void QueueMsg::Cache::callread ()
{
	int rc = read (sd, buf, sizeof(buf) - 1); 
	if ( rc == -1 ) {
		perror("Error read().\n");
	}
	if ( rc == 0 ) {
		printf("Server closed connection.\n");
		exit(1);
	}

	buf[rc] = '\0';
	for(int i = 0; i < rc; i++){
		if ( buf[i] == '\n' ) {
			cnt++;
		}
	}
}



char *QueueMsg::Cache::getmsg () 
{
	if ( cnt == 0) {
		callread ();
	}

	if ( cnt != 0 ) {
		appendext (buf, ext);
		pasteext (ext, msg);
		cutext (ext);
		cnt--;
	}
	
	return msg;
}



void QueueMsg::Cache::sendstr (char *str) const
{
	char strn[32];

	sprintf (strn, "%s\n", str);
	printf ("Now send:[%s].\n", strn);
	write (sd, strn, strlen(strn));
}



/* */
void QueueMsg::Cache::pasteext (char *str1, char *str2)
{
	int i = 0;
	while ( str1[i] != '\n' ) {
		str2[i] = str1[i];
		i++;
	}

	str2[i] = '\0';
}



/* */
void QueueMsg::Cache::cutext (char* str)
{
	int i = 0;
	while ( str[i++] != '\n' ); 

	int k = 0;
	do {
		str[k++] = str[i++];
	} while ( str[k-1] != '\0');
}



/* */
void QueueMsg::Cache::appendext (char *str1, char *str2)
{
	int i = 0;
	while ( str2[i++] != '\0' );
	i--;	

	int k = 0;
	do {
		str2[i++] = str1[k++];
	} while ( str2[k-1] != '\0' );

	str1[0] = '\0';
}



QueueMsg::Cache:: ~Cache ()
{
}



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
	printf ("============.\n");
	QueueElem * f = first;
	while ( f != 0 ) {
		printf ("msg:[%s]\n", f->str);
		f = f->next;
	}
	
	printf ("============.\n");
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
