#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/times.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



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
class Descriptor{
	int fd;
	char buf[1024];
public:
	DArray(){
	}
};


/* */
class Game{
	int fd[2] = {0, 0};	// fd[0] - ls, fd[1] - keyboard.
	Descriptor d1, d2;
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
	Game(int desc, char *n, int r);
private:
	void iteration();
	void login(char *nick, int room);
};



/* */
Game::Game(int ls, char *n, int r) 
	: fd[0](ls), nick(n), room(r)
{
	printf("Nick:[%s]\nRoom:[%d]\n", nick, room);
	
	printf("Main cycle of wait data.\n");
	for (;;){
		iteration();
	}


	printf("If login are working, can PLAY.\n");
}



/* */
void Game::iteration()
{
	fd_set readfds;

	FD_ZERO(&readfds)
	FD_SET(fd[0], &readfds);
	FD_SET(fd[1], &readfds);

	max_d = max(fd[0], fd[1]);
	
	int res = select(max_d + 1, &readfds, NULL, NULL, NULL);
	if (res < 1){
		perror("Error select.\n");
	}

	for( int i = 0; i <= 1; i++) {
		if ( FD_ISSET(fd[i], readfds) ) {
			ReadToBuffer(fd[i]);
		}
	}
}



/* */
void Game::login(char *nick, int room)
{
	printf("Method Game::login.\n");
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
int main(int argc, char **argv)
{	
	printf("Start program.\n");
	
	char* ip;
	int port;
	char *nick;
	int room;

	ParseArguments(argc, argv, ip, port, nick, room);
	
	Socket link(ip, port);
	int fd = link.get_sockfd();
	printf("Listen socket:[%d].\n", fd);		
		

	Game g(fd, nick, room);

	printf("End program.\n");
	return 0;
}

