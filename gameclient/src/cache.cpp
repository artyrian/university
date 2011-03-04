#include "cache.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


Cache::Cache (char *ip, int port) 
	: link (ip, port), cnt(0)
{
	sd = link.connecting ();
	printf ("Now connecting to sd[%d].\n", sd);
	msg[0] = '\0';
	buf[0] = '\0';
	ext[0] = '\0';
}



void Cache::callread ()
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



char *Cache::getmsg () 
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
	
	if ( msg[0] != '*' && msg[0] != '#' ) {
		printf ("Now read msg:[%s].\n", msg);
	}

	return msg;
}

void Cache::sendstr (char *str) const
{
	char strn[32];

	sprintf (strn, "%s\n", str);
	printf ("Now send:[%s].\n", strn);
	write (sd, strn, strlen(strn));
}

/* */
void Cache::pasteext (char *str1, char *str2)
{
	int i = 0;
	while ( str1[i] != '\n' ) {
		str2[i] = str1[i];
		i++;
	}

	str2[i] = '\0';
}



/* */
void Cache::cutext (char* str)
{
	int i = 0;
	while ( str[i++] != '\n' ); 

	int k = 0;
	do {
		str[k++] = str[i++];
	} while ( str[k-1] != '\0');
}



/* */
void Cache::appendext (char *str1, char *str2)
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


Cache::~Cache ()
{
}

