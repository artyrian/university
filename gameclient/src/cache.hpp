#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include "socket.hpp"


class QueueMsg {
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
	QueueMsg (char *ip, int port) ;
	char *gettype (int type = 0);
	char * readqueue ();
	void sendstr (char *str) const;
	int getcount () const;

	~QueueMsg (); 
};

#endif
