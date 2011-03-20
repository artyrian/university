#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include "socket.hpp"

class Cache {
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

#endif
