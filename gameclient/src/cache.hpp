#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include "socket.hpp"

class Cache{
	Socket link;

	int sd;
	char buf[1024];
	char msg[1024];
	char ext[2048];

	int cnt;

	void callread ();
	void pasteext (char *, char *);
	void appendext (char *, char *);
	void cutext (char *);
public:
	Cache (char *ip, int port);

	char *getmsg ();
	void sendstr (char *) const;

	~Cache ();
};


/*
class QueueMsg {
	Cache *ch;

	struct QueueElem{
		char *str;
		int type;
		Item *next;
	};

	QueueElem *first, *last;

	int cnt;

	char *findspecialmsg (int type) {
		QueueElem *cur = first;
		QueueElem *previous = first;

		char msg[80] = '\0';

		while ( cur != 0 ) {
			if ( cur->type == type ) {
				msg = cur->str;
				break;
			}
			previous = cur;
			cur = cur->next;
		}

		previous->next = cur->next;

		return msg;
	}

	int typing (*str) {

	}
public:
	QueueMsg (char *ip, int port)
		: ch (ip, port), first (0), last (0)
	{
	}

	char *gettype (int type = 0) {
		char msg[80];

		if ( type == 0 ) {
			return findspecialmsg ();
		} else if ( type == typing (msg = ch.getmsg) ) {
			return 	msg;
		}

	~QueueMsg ();
};
*/



#endif
