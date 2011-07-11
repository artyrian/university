#include "lexer.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


static int get_symbol ();
static ListElem * new_element ( );
static int put_lex (ListElem *);
static int isdelim (int);
static ListElem * feed_symbol (int, Buffer *);
static ListElem * step (int, Buffer *);
static ListElem * state_H (int, Buffer *);
static ListElem * state_D (int, Buffer *);
static ListElem * state_Q (int, Buffer *);
static ListElem * state_B (int, Buffer *);
static ListElem * state_N (int, Buffer *);
static ListElem * state_W (int, Buffer *);

static enum {
	H, D, Q, B, N, W
} CS;


int get_symbol ()
{
	return getchar ();
}


ListElem * new_element (Buffer * buf)
{
	ListElem * new = malloc (sizeof (ListElem));
	new->string = malloc (buf->cnt);
	strcpy (new->string, buf->string);
	new->lenght = buf->cnt;
	new->next = NULL;

	flush_buffer (buf);

	return new;
}


int put_lex (ListElem * cur)
{
	printf ("[%s]:%d\n", cur->string, cur->lenght);

	return 0;
}


int isdelim (int c)
{
	if ( c == '|' || c == '<' || c == '>' || c == '&') {
		return c;
	}
	
	return 0;
}


int fill_list ()
{
	int c;
	ListElem * elem;
	Buffer * buf = malloc (sizeof (Buffer));

	init_buffer (buf);

	while ( 1 ) {
		if ( (c = get_symbol ()) == EOF ) {
			break;
		}
		if ( (elem = feed_symbol (c, buf)) != NULL) {
			put_lex (elem);
		}
	}
	if ( (elem = feed_symbol (' ', buf)) != NULL ) {
		put_lex (elem);
	}

	free_buffer (buf);
	free (buf);

	return 0;
}


ListElem * feed_symbol (int c, Buffer * buf)
{
	ListElem * result;

	result = step (c, buf);
	if ( result != NULL ) {
		step (c, buf);		// ?->H
	}
	
	return result;
}


ListElem * step (int c, Buffer * buf)
{
	switch ( CS ) {
	case H:
		return state_H (c, buf);
	case D:
		return state_D (c, buf);
	case Q:
		return state_Q (c, buf);
	case B:
		return state_B (c, buf);
	case N:
		return state_N (c, buf);
	case W:
		return state_W (c, buf);
	default:
		printf ("Error in switch. Non-real state.\n");
		exit (1);
	}
}


ListElem * state_H (int c, Buffer * buf)
{
	if ( isspace (c) ) {
		return NULL;
	}
	else if ( isdelim (c) ) {
		add_symbol (c, buf);
		CS = D;
		return NULL;
	}
	else if ( c == '\\' ) {
		CS = B;
		return NULL;
	}
	else if ( c == '"' ) {
		CS = Q;
		return NULL;
	}
	else {
		add_symbol (c, buf);
		CS = W;
		return NULL;
	}
}


ListElem * state_D (int c, Buffer * buf)
{
	if ( isdelim (c) ) {
		add_symbol (c, buf);
		return NULL;
	}
	else {
		CS = H;
		return new_element (buf);
	}
}


ListElem * state_Q (int c, Buffer * buf)
{
	if ( c == '"' ) {
		CS = N;
		return NULL;
	}
	else {
		add_symbol (c, buf);
		return NULL;
	}
}


ListElem * state_N (int c, Buffer * buf)
{
	if ( isspace (c) ) {
		CS = H;
		return new_element (buf);
	}
	else if ( c == '"') {
		CS = Q;
		return NULL;
	}
	else if ( c == '\\' ) {
		CS = B;
		return NULL;
	}
	else {
		add_symbol (c, buf);
		CS = W;
		return NULL;
	}
}


ListElem * state_B (int c, Buffer * buf)
{
	if ( c == EOF ) {
		printf ("Error: EOF in state B.\n");
		exit (1);
	}
	add_symbol (c, buf);
	CS = N;

	return NULL;
}


ListElem * state_W (int c, Buffer * buf)
{
	if ( isspace (c) ) {
		CS = H;
		return new_element (buf);
	}
	else if ( c == '"') {
		CS = Q;
		return NULL;
	}
	else if ( c == '\\') {
		CS = B;
		return NULL;
	}
	else if ( isdelim (c) ) {
		CS = D;	
		return new_element (buf);
	}
	else {
		add_symbol (c, buf);
		return NULL;
	}
}
