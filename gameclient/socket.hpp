#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <netinet/in.h>

class Socket {
	struct sockaddr_in addr;
	char *ip;
	int port;
public:
	Socket (char* i, int p);
	int connecting ();
private:
	int createsocket();
	void init_ip(char *arg);
	void init_port(int arg);
};

#endif
