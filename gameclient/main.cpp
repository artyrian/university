#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/times.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

void pasteext (char*, char*);
void cutext (char*);
void appendext (char*, char*);


/*
 * Module SOCKET
 */
class Socket{
	struct sockaddr_in addr;
	char *ip;
	int port;
	int sd;
public:
	Socket (char* i, int p);
	int connecting ();
private:
	int createsocket();
	void init_ip(char *arg);
	void init_port(int arg);
};


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
		printf("IP:[%s]", ip);
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
	int ls;
	if ( -1 == (ls = socket(AF_INET, SOCK_STREAM, 0)) ){
		perror("Error create socket.\n");
	}
	return ls;
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

/*
 * End of module SOCKET;
 */


/* */
class Cache{
	Socket link;
public:
	int sd;			// MUST BE HIDDEN!
	char buf[1024];		// must be hidden!
	char msg[1024];		// must be hidden!
	char ext[2048];		// must be hidden!
	int cnt;	// cnt \n;

	Cache (char *ip, int port) 
		: link (ip, port), cnt(0)
	{
		sd = link.connecting ();
		Flushstr (buf);
		Flushstr (msg);
		Flushstr (ext);
	}

	void Flushstr(char *str) {	// maybe shoud be hidden??
		str[0] = '\0';
	}
};


/* */
class Game {
	Cache ch;
	char *nick;
	int room;
public:
	Game (char *ip, int port, char *n, int r);

	void send (char *);
	void waitsymbol (int, char);
private:
	void callread (int index);	// this
	void readportion (int index);	// and this must be in Cache !!!
};





/* */
Game::Game (char *ip, int port, char *n, int r)
	: ch (ip, port), nick (n), room (r)
{
	char str[32];
	
	send (nick);

	sprintf (str, ".join %d", room);
	send (str);

	printf ("I wait & for start.\n");
	waitsymbol (0, '&');	
	
	printf ("Done.My str [%s].\n", ch.msg);
}




/* */
void Game::send (char *fn)
{
	char str[32];

	sprintf (str, "%s\n", fn);
	printf ("Now send:[%s].\n", str);
	write (ch.sd, str, strlen(str));
}




/* */
void Game::waitsymbol (int idx, char p)
{
	ch.Flushstr (ch.msg);

	while ( ch.msg[0] != p ) {
		if ( ch.cnt == 0) {
			callread (idx);	
		} else {
			appendext (ch.buf, ch.ext);
			ch.Flushstr (ch.buf);
			pasteext (ch.ext, ch.msg);
			cutext (ch.ext);
			ch.cnt--;
		}
	}

	printf ("while cnt::\nbuf[%s]\next[%s]\nmsg[%s]\ncnt:%d.\n", 
		ch.buf, 
		ch.ext, 
		ch.msg, 
		ch.cnt);
}



void Game::callread (int idx)
{
	int rc = read (ch.sd, ch.buf, sizeof(ch.buf) - 1); 
	if ( rc == -1 ) {
		perror("Error read().\n");
	}
	if ( rc == 0 ) {
		printf("Server closed connection.\n");
		exit(1);
	}

	ch.buf[rc] = '\0';
	for(int i = 0; i < rc; i++){
		if ( ch.buf[i] == '\n' ) {
			ch.cnt++;
		}
	}
}



/* */
void pasteext (char *str1, char *str2)
{
	int i = 0;
	while ( str1[i] != '\n' ) {
		str2[i] = str1[i];
		i++;
	}

	str2[i] = '\0';
}



/* */
void cutext (char* str)
{
	int i = 0;
	while ( str[i++] != '\n' ); 

	int k = 0;
	do {
		str[k++] = str[i++];
	} while ( str[k-1] != '\0');
}



/* */
void appendext (char *str1, char *str2)
{
	int i = 0;
	while ( str2[i++] != '\0' );
	i--;	

	int k = 0;
	do {
		str2[i++] = str1[k++];
	} while ( str2[k-1] != '\0' );
}


/* */
void ParseArguments(	int n, char **argv, 
			char*& ip, int& port, char*& nick, int& room)
{
	if ( n != 5 ){
		printf ("WARNING!!! Enabled debug mode.\nStatic parametrs (ip, port, nick, room).\n");
		port = 4774; 
		room = 1; 
		strcpy (ip, "0"); 
		strcpy (nick, "Bot0"); 
	} else { 
		ip = argv[1];
		port = atoi(argv[2]);
		nick = argv[3];
		room = atoi(argv[4]);
	}
}



/* */
int main(int argc, char **argv)
{	
	printf("Start program.\n");
	
	char *ip = (char *) malloc(16);
	int port;
	char *nick = (char *) malloc(22);
	int room;

	ParseArguments (argc, argv, ip, port, nick, room);
	
	Game g(ip, port, nick, room);

	for (;;) {
		printf ("Here a loop where i send cmd every 5 sec.\n");

/*
		char str [32];

		sprintf (str, "market");
		g.send (str);
	

		printf ("I wait & for market.\n");
		g.waitsymbol (0, '&');
		printf ("Done. My str [%s].\n", ch.msg);

		sprintf (str, "buy 2 500");
		g.send (str);

		printf ("I wait & for buy.\n");
		g.waitsymbol (0, '&');
		printf ("Done. My str [%s].\n", ch.msg);
*/
		sleep (5);
	}

	free (ip);
	free (nick);

	printf("End program.\n");
	return 0;
}
