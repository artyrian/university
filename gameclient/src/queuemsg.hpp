#ifndef _QUEUEMSG_HPP_
#define _QUEUEMSG_HPP_

#include "cache.hpp"


class QueueMsg {
	Cache ch;

	struct QueueElem {
		char *str;
		int type;
		QueueElem *next;
	};
	QueueElem *first, *last;

	int cnt;

	char msgq [1024];
	char msg[1024];

	QueueElem * create (char *str);
	void add (char *str);
	void remove ();
	int comment (const char c) const;
public:
	QueueMsg (char *ip, int port);

	char * gettype (int type);

	char * getmsgq ();
	int getcount () const;

	void sendstr (char *str) const;

	~QueueMsg (); 
};

#endif
