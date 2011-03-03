#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include "socket.hpp"

class Cache{
	Socket link;

	int sd;
	char buf[1024];		// must be hidden!
	char msg[1024];		// must be hidden!
	char ext[2048];		// must be hidden!

	int cnt;

	void callread ();
	void pasteext (char *, char *);
	void appendext (char *, char *);
	void cutext (char *);
public:
	Cache (char *ip, int port);

	char *getmsg ();
	void sendstr (char *);
};

#endif
