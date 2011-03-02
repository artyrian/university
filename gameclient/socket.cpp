#include "socket.hpp"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>

#include <stdio.h>		// but it while not CATCH!!!
#include <stdlib.h>		// but it while not CATCH!!!
				// OR MAYBE IT'S NORMAL BECAUSE CRITITCAL ERRORS.

Socket::Socket(char* i, int p) : ip(i), port(p)
{
	addr.sin_family = AF_INET;
	init_port (port);
	init_ip (ip);
}


void Socket::init_ip(char *arg)
{
	ip = arg;
	if ( !inet_aton(ip, &(addr.sin_addr)) ){
		perror ("Invalid IP address!\n");
		exit(1);
	}
}


void Socket::init_port(int arg)
{
	port = arg;
	addr.sin_port = htons(port);
}


int Socket::createsocket()
{
	int sd;
	if ( -1 == (sd = socket(AF_INET, SOCK_STREAM, 0)) ){
		perror("Error create socket.\n");
	}

	fcntl (sd, O_NONBLOCK);

	return sd;
}


int Socket::connecting ()
{
	int sd = createsocket();

	if ( 0 != connect(sd, (struct sockaddr *)&addr, sizeof(addr)) ){
		perror ("Connect error.");
		exit(1);
	}

	return sd;
}
