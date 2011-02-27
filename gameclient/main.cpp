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
void pastebuf (char*, char*);
void cutbuf (char*);
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
	char ext[1024];
	int cnt;	// cnt;
	bool fmsg; 	// flag of ready message;
	bool fprc;

	void FlushBuf () {
		buf[0] = '\0';
	}

	void FlushMsg() {
		msg[0] = '\0';
		fmsg = 0;
	}
	
	void FlushExt () {
		ext[0] = '\0';
	}

	void Flush () {
		cnt = 0;
		FlushBuf ();
		FlushMsg ();
		FlushExt ();
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
	void login();
	void iteration();
	void callread (int index);
	void readportion(int index);
	void readstr (int index);
	void parse();
};



/* */
void Game::readstr (int idx)
{
	while ( ca[idx].fmsg != 1 ) {
		readportion (idx);
	}

	if ( ca[idx].msg[0] == '%' ) {
		ca[idx].fprc = 1;
	}
}



/* */
Game::Game(int ls, char *n, int r) 
{
	ca[0].Init (ls);
	ca[1].Init (STDIN_FILENO);

	nick = n;
	room = r;

	printf("Nick:[%s]\nRoom:[%d]\n", nick, room);


	login();
	
	printf("Main cycle of wait data.\n");
	for (;;){
		iteration();
	}
}



/* */
void Game::login()
{
	char str[32];
	readstr (0);
	printf ("Now in msg:[%s], Must be (Please input nick: )\n", ca[0].msg);

	ca[0].Flush ();

	sprintf (str, "%s\n", nick);
	printf ("Now str[%s]\n", str);
	write (ca[0].fd, str, strlen(str));
		
	readstr (0);
	printf ("ext:[%s],\nmsg[%s],\nfmsg[%d],\nfprc[%d].\n", ca[0].ext, ca[0].msg, ca[0].fmsg, ca[0].fprc);
		
	if ( ca[0].msg[1] == '-' ) {
		perror ("This nick has already used, try other.\n");	
		exit (1);
	}

	ca[0].Flush ();
	
	// read info;
	
	sprintf (str, ".join %d\n", room);
	printf ("Now str[%s]\n", str);
	write (ca[0].fd, str, strlen(str)); 
	
	// check

	readstr (0);

	printf ("ext:[%s],\nmsg[%s],\nfmsg[%d],\nfprc[%d].\n", ca[0].ext, ca[0].msg, ca[0].fmsg, ca[0].fprc);
		
	if ( ca[0].msg[1] == '-' ) {
		perror ("This room not exits, try other.\n");	
		exit (1);
	}

	ca[0].Flush ();
}



/* */
void Game::iteration()
{
<<<<<<< HEAD
	printf("Method Game::login.\n");

	char buf[5];

	read(fd, buf, 5);
	buf[5] = '\0';	

	printf("Now I read from fd[%d] str[%s]", fd, buf);
	
	char buf[20];
=======
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(ca[0].fd, &readfds);
	FD_SET(ca[1].fd, &readfds);

<<<<<<< HEAD
	max_d = max(fd[0], fd[1]);
>>>>>>> 73a624fc927a7c69c63277dcaec0d572738811ab
=======
	int max_d = max(ca[0].fd, ca[1].fd);
>>>>>>> 9f9dab505c1cf1d8854ca5877a970488b3157f9b
	
	int res = select(max_d + 1, &readfds, NULL, NULL, NULL);
	if (res < 1){
		perror("Error select.\n");
	}

	for( int i = 0; i <= 1; i++ ) {
		if (FD_ISSET (ca[i].fd, &readfds)) {
			if ( ca[i].fmsg == 0 ) {	// read if we havent msg.
				readportion(i);
			}
		}
		
		if ( ca[i].msg[0] != '\0' ) {
			// can parse ca[i].msg;
			parse();
			// return type, argc, argv.; 
		}
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
		if ( ca[idx].buf[i] ) {
			ca[idx].cnt++;
		}
	}
}



/* */
void Game::readportion(int idx)
{
	if ( ca[idx].cnt == 0 ) {
		callread (idx);
	}
	else
	{
		pasteext (ca[idx].ext, ca[idx].msg);
		pastebuf (ca[idx].buf, ca[idx].msg);
		cutbuf (ca[idx].buf);
		appendext (ca[idx].buf, ca[idx].ext);
		ca[idx].cnt--;
		ca[idx].fmsg = 1;
	} 
}

<<<<<<< HEAD
	sprintf(buf, ".join %d", room);
	write(fd, buf, strlen(buf) + 1);
	//check room
=======


/* */
void Game::parse()
{
<<<<<<< HEAD
	printf("Method Game::login.\n");
>>>>>>> 73a624fc927a7c69c63277dcaec0d572738811ab
=======

}



/* */
void pasteext (char *str1, char *str2)
{
	int i = 0;
	while ( str1[i] != '\0' ) {
		str2[i] = str1[i];
		i++;
	}
}

/* */
void pastebuf (char *str1, char *str2)
{
	int i = 0;
	while ( str2[i++] != '\0' );
	i--;

	int k = 0;
	while ( str1[k] != '\n' && str1[k] != '\0' ) {
		str2[i++] = str1[k++];	
	}
>>>>>>> 9f9dab505c1cf1d8854ca5877a970488b3157f9b
}



/* */
void cutbuf (char* str)
{
	int i = 0;
	while ( str[i++] != '\n' ); 

	int k = 0;
	while ( str[i] != '\0' ) {
		str[k++] = str[i++];
	}
}



/* */
void appendext (char *str1, char *str2)
{
	int i = 0;
	while ( str2[i++] != '\0' );
	i--;	

	int k = 0;
	while ( str1[k] != '\0' ) {
		str2[i++] = str1[k++];
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
	strcpy (nick, "Bot0");
	
	Socket link(ip, port);
	int fd = link.get_sockfd();
	printf("Listen socket:[%d].\n", fd);		
		

	Game g(fd, nick, room);

	free (ip);
	free (nick);

	printf("End program.\n");
	return 0;
}
