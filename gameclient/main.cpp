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

/* */
class Socket{
	struct sockaddr_in addr;
	char* ip;
	int port;
	int sockfd;
public:
	Socket(char* i, int p) : ip(i), port(p) {
		sockfd = createsocket();
		addr.sin_family = AF_INET;
		init_port(port);
		init_ip(ip);
		connecting(sockfd);
	}
	int get_sockfd(){
		return sockfd;	
	}
private:
	int createsocket(){
		int ls;
		if ( -1 == (ls = socket(AF_INET, SOCK_STREAM, 0)) ){
			perror("Error create socket.\n");
		}
		return ls;
	}
	void init_ip(char *arg){
		ip = arg;
		if ( !inet_aton(ip, &(addr.sin_addr)) ){
			printf("IP:[%s]", ip);
			perror ("Invalid IP address!\n");
			exit(1);
		}
	}
	void init_port(int arg){
		port = arg;
		addr.sin_port = htons(port);
	}
	void connecting(int sockfd){
		if ( 0 != connect(sockfd, (struct sockaddr *)&addr, 
							sizeof(addr)) ){
			perror ("Connect error.");
			exit(1);
		}
	}
};



/* */
struct Cache{
	int fd;
	char buf[1024];
	char msg[1024];
	char ext[2048];
	int cnt;	// cnt;
	int fmsg; 	// flag of ready message;
	int fprc;

	void Flushstr(char *str) {
		str[0] = '\0';
	}

	void Flush () {
		cnt = 0;
		Flushstr (buf);
		Flushstr (msg);
//		Flushstr (ext);
		fmsg = 0;
		fprc = 0;
	}
	
	void Init(int fd_p) {
		fd = fd_p; 
		Flush ();
	}
};


/* */
class Game{
	Cache ca[2];	// 0 - ls, 1 - keyboard;
	char *nick;
	int room;

	int max(int a, int b){
		if ( a > b){
			return a;
		} else {
			return b;
		}
	}
public:
	Game(int ls, char *n, int r);
private:
	void send (char *);
	void waitsymbol (char);
	void check (char *);
	void iteration ();
	void callread (int index);
	void readportion (int index);
	void readstr (int index);
	void parse ();

	void market ();
};





/* */
Game::Game(int ls, char *n, int r) 
{
	ca[0].Init (ls),
	ca[1].Init (STDIN_FILENO),
	nick = n,
	room = r;

	char str[32];
	
	send (nick);
	

	sprintf (str, ".join %d", room);
	send (str);

	printf ("I wait & for start.\n");
	waitsymbol ('&');	
	printf ("Done.\n");

//
	sprintf (str, "market");
	send (str);

	printf ("I wait & for market.\n");
	waitsymbol ('&');
	printf ("Done.\n");


	sleep (5);

	sprintf (str, "buy 2 500");
	send (str);
//
/*
	printf("Main cycle of wait data.\n");
	for (;;){
		iteration();
	}
*/
}




/* */
void Game::send (char *fn)
{
	char str[32];

	sprintf (str, "%s\n", fn);
	printf ("Now send:[%s].\n", str);
	write (ca[0].fd, str, strlen(str));
}




/* */
void Game::waitsymbol (char p)
{
	ca[0].Flush ();
	do {
		readstr (0);
	} while ( ca[0].msg[0] != p); 
}



/* */
void Game::check (char *fn)
{
	if ( ca[0].fprc == 1 && ca[0].msg[1] == '-' ) {
		printf ("Error in fn (%s)", fn);
		exit (1);
	}
}



/* */
void Game::iteration()
{
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(ca[0].fd, &readfds);
	FD_SET(ca[1].fd, &readfds);

	int max_d = max(ca[0].fd, ca[1].fd);
	
	int res = select(max_d + 1, &readfds, NULL, NULL, NULL);
	if (res < 1){
		perror("Error select.\n");
	}

	for( int i = 0; i <= 1; i++ ) {
		if (FD_ISSET (ca[i].fd, &readfds)) {
			// here read.
		}
		// here parse.
	}

}



void Game::callread (int idx)
{
	int rc = read (ca[idx].fd, ca[idx].buf, sizeof(ca[idx].buf) - 1); 
	if ( rc == -1 ) {
		perror("Error read().\n");
	}
	if ( rc == 0 ) {
		printf("Server closed connection.\n");
		exit(1);
	}

	ca[idx].buf[rc] = '\0';
	for(int i = 0; i < rc; i++){
		if ( ca[idx].buf[i] == '\n' ) {
			ca[idx].cnt++;
		}
	}
}



/* */
void Game::readstr (int idx)
{
	if ( ca[idx].cnt == 0 ) {
		callread (idx);
	}

	while ( ca[idx].cnt != 0 ) {
		appendext (ca[idx].buf, ca[idx].ext);
		ca[idx].Flushstr (ca[idx].buf);
		pasteext (ca[idx].ext, ca[idx].msg);
		cutext (ca[idx].ext);
		ca[idx].cnt--;

		printf ("while cnt::\nbuf[%s]\next[%s]\nmsg[%s]\ncnt:%d", ca[idx].buf, ca[idx].ext, ca[idx].msg, ca[idx].cnt);
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
void ParseArguments(	int n, char** argv, 
			char*& ip, int& port, char*& nick, int& room)
{
	if ( n != 5 ){
		perror("Too few arguments!\n");
		exit(1);
	} else { 
		ip = argv[1];
		port = atoi(argv[2]);
		nick = argv[3];
		room = atoi(argv[4]);
	}
}



/* */
void ReadConf (char*& ip, int& port, char*& nick, int& room)
{
	FILE *f;
	char strprm[32];
	if ( (f = fopen("conf", "r")) == 0) {
		perror ("Error reading conf");
	}

	int i, c = '\n';
	for (int k = 0; k < 4; k++) {
		i = 0;
		while ( c != ';' ) {
			c = fgetc (f);
			strprm[i++] = c;
		}
		c = fgetc (f);
		strprm[--i] = '\0';

		switch (k) { 
			case 0: strcpy(ip, strprm); break;
			case 1: port = atoi(strprm); break;
			case 2: strcpy(nick, strprm); break; 
			case 3: room = atoi(strprm); break;
		}	
	}
}



/* */
int main(int argc, char **argv)
{	
	printf("Start program.\n");
	const int conf = 0;
	
	char *ip = (char *) malloc(16);
	int port;
	char *nick = (char *) malloc(22);
	int room;

	if ( conf == 1 ) {
		ReadConf (ip, port, nick, room);
	} else {
	//	ParseArguments (argc, argv, ip, port, nick, room);

		port = 4774;
		room = 1;
		strcpy (ip, "0");
		strcpy (nick, "Bot0");
	}
	
	Socket link(ip, port);
	int fd = link.get_sockfd();
	printf("Listen socket:[%d].\n", fd);		
		
	Game g(fd, nick, room);

	free (ip);
	free (nick);

	printf("End program.\n");
	return 0;
}
