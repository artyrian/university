#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <netinet/in.h>

class Socket {
	struct sockaddr_in addr;
	char *ip;
	int port;
	int sd;
	
	void init_ip(char *arg);
	void init_port(int arg);
	void createsocket();

	char buf[1024];
	char msg[1024];
	char ext[2048];

	int cnt;

	void callread ();
	void pasteext (char *, char *);
	void appendext (char *, char *);
	void cutext (char *);
public:
	Socket (char* i, int p);
	void connecting ();
	~Socket ();

	char *getmsg ();
	void sendstr (char *) const;
};

#endif
