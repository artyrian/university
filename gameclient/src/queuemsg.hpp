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

	char *msgq;
	char *msg;

	QueueElem * create (char *str);
	void add (char *str);
	void remove ();
	int comment (const char c) const;
public:
	QueueMsg (char *ip, int port);

	char * gettype (int type = 0);
	char * readqueue ();
	void deletemsgq ();
	int getcount () const;

	void sendstr (char *str) const;

	~QueueMsg (); 
};

#endif
