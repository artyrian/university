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

void copy (char*, char*, char);
void cut (char*);

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
	char msg[1024];
	char ext[1024];
	int cnt;	// cnt;
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
	void login();
	void iteration();
	void readportion(int fd);
	void parse();
};



/* */
Game::Game(int ls, char *n, int r) 
{
	ca[0].fd = ls; 
	ca[1].fd = STDIN_FILENO;
	ca[0].cnt = ca[1].cnt = 0;
	ca[0].msg[0] = ca[1].msg[0] = '\0';
	ca[0].ext[0] = ca[1].ext[0] = '\0';

	nick = n;
	room = r;

	printf("Nick:[%s]\nRoom:[%d]\n", nick, room);

	login();
	
	printf("Main cycle of wait data.\n");
	for (;;){
		iteration();
	}


	printf("If login are working, can PLAY.\n");
}



/* */
void Game::login()
{
	char buf[1024];
	read (ca[0].fd, buf, sizeof(buf) - 1);
	buf[sizeof(buf)] = '\0';
	printf ("Some text:[%s].\n", buf);

	printf ("Try login to server with nick[%s].\n", nick);
	nick[strlen(nick)] = '\n';

	write (ca[0].fd, nick, strlen(nick));
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
			readportion(i);
		}
		
		if ( ca[i].msg[0] != '\0' ) {
			// can parse ca[i].msg;
			parse();
			// return type, argc, argv.; 
		}
	}

}



/* */
void Game::readportion(int idx)
{
	char buf[1024];

	int rc = read (ca[idx].fd, buf, sizeof(buf) - 1); 
	if ( rc == -1 ) {
		perror("Error read().\n");
	}
	if ( rc == 0 ) {
		printf("Server closed connection.\n");
		exit(1);
	}

	buf[rc] = '\0';
	for(int i = 0; i < rc; i++){
		if ( buf[i] ) {
			ca[idx].cnt++;
		}
	}

	if ( ca[idx].cnt != 0 ) {
		copy (ca[idx].ext, ca[idx].msg, '\n');
		copy (buf, ca[idx].msg, '\n');
		cut (buf);
		ca[idx].cnt--;
	} else {
		copy (buf, ca[idx].ext, '\0');
	}
}



/* */
void Game::parse()
{

}



/* */
void cut (char* str)
{
	int i = 0;
	while ( str[i++] != '\n' ); 

	int k = 0;
	while ( str[i] != '\0' ) {
		str[k++] = str[i++];
	}
}



/* */
void copy (char* str1, char* str2, char stopsymbol)
{

 	int k = 0;
	// ATTENTION!!!
	while ((str2[k] != stopsymbol) && (str2[k] != '\0')) {
		k++;
	}
	k--;

	int i = 0;
	// ATTENTION!!!
	while ((str1[i] != stopsymbol) && (str1[i] != '\0')) {
		str2[k++] = str1[i++];
	}
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
	char strarg[32], strprm[32];
	if ( (f = fopen("conf", "r")) == 0) {
		perror ("Error reading conf");
	}

	int i, c = '\n';
	for (int k = 0; k < 4; k++) {
		i = 0;
		while ( c != '=' ) {
			c = fgetc (f);
			strarg[i++] = c;
		}
		strarg[--i] = '\0';
		printf ("Now i read [%s].\n", strarg);

		i = 0;
		while ( c != ';' ) {
			c = fgetc (f);
			strprm[i++] = c;
		}
		c = fgetc (f);
		strprm[--i] = '\0';
		printf ("Now i read [%s].\n", strprm);

		switch (strarg[0]) { 
			case 'I': strcpy(strprm, ip); break;
			case 'P': port = atoi(strprm); break;
			case 'N': strcpy(strprm, nick); break; 
			case 'R': room = atoi(strprm); break;
			default: perror("Mistake in conf.\n"); 
		}	
	}
}



/* */
int main(int argc, char **argv)
{	
	printf("Start program.\n");
	
	char *ip;
	int port;
	char *nick;
	int room;
/*
	if ( argc == 1) {
		ReadConf (ip, port, nick, room);
	} else {
		ParseArguments (argc, argv, ip, port, nick, room);
	}
*/
//	ParseArguments (argc, argv, ip, port, nick, room);
	port = 4774;
	room = 1;
	ip = (char *) malloc(15);
	nick = (char *) malloc(22);
	strcpy (ip, "0");
	strcpy (nick, "Bot");
	
	Socket link(ip, port);
	int fd = link.get_sockfd();
	printf("Listen socket:[%d].\n", fd);		
		

	Game g(fd, nick, room);

	free (ip);
	free (nick);

	printf("End program.\n");
	return 0;
}

