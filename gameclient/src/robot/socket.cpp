#include "socket.hpp"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>


Socket:: Socket(char* i, int p) 
{
	ip = i;
	port = p;
	addr.sin_family = AF_INET;
	init_port (port);
	init_ip (ip);

	connecting ();

	printf ("Now connecting to sd[%d].\n", sd);

	msg[0] = '\0';
	buf[0] = '\0';
	ext[0] = '\0';
}


void Socket:: connecting ()
{
	if ( (sd = socket (AF_INET, SOCK_STREAM, 0)) == -1 ) {
		perror("Error create socket.\n");
	}

	if ( connect (sd, (struct sockaddr *)&addr, sizeof(addr)) != 0 ) {
		perror ("Connect error.");
		exit (EXIT_FAILURE);
	}
}


Socket:: ~Socket ()
{
	shutdown (sd, 2);
	close (sd);
}


void Socket:: init_ip(char *arg)
{
	ip = arg;
	if ( !inet_aton(ip, &(addr.sin_addr)) ) {
		perror ("Invalid IP address!\n");
		exit(1);
	}
}


void Socket:: init_port(int arg)
{
	port = arg;
	addr.sin_port = htons(port);
}


void Socket:: callread ()
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


/* */
void Socket:: pasteext (char *str1, char *str2)
{
	int i = 0;
	while ( str1[i] != '\n' ) {
		str2[i] = str1[i];
		i++;
	}

	str2[i] = '\0';
}


/* */
void Socket:: cutext (char* str)
{
	int i = 0;
	while ( str[i++] != '\n' ); 

	int k = 0;
	do {
		str[k++] = str[i++];
	} while ( str[k-1] != '\0');
}


/* */
void Socket:: appendext (char *str1, char *str2)
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


char *Socket:: getmsg () 
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
	
//	printf ("R:[%s].\n", msg);

	return msg;
}


void Socket:: sendstr (char *str) const
{
	char strn[32];

	sprintf (strn, "%s\n", str);
	printf ("Now send:[%s].\n", strn);
	write (sd, strn, strlen(strn));
}
