#include "lexer.h"
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


static int get_symbol ();
static ListElem * new_element (Buffer *, TypeLex);
static void put_lex (ListElem *, ListElem **);
static int isdelim (int);
static TypeLex typify (Buffer *);
static ListElem * feed_symbol (int, Buffer *);
static ListElem * step (int, Buffer *);
static ListElem * state_H (int, Buffer *);
static ListElem * state_D (int, Buffer *);
static ListElem * state_Q (int, Buffer *);
static ListElem * state_B (int, Buffer *);
static ListElem * state_N (int, Buffer *);
static ListElem * state_W (int, Buffer *);
static ListElem * Sp (ListElem *);
static ListElem * Bp (ListElem *);
static ListElem * Fp (ListElem *);
static ListElem * Pp (ListElem *);
static ListElem * Cp (ListElem *);
static ListElem * Dp (ListElem *);
static ListElem * Rp (ListElem *);
static ListElem * Wp (ListElem *);
static ListElem * word (ListElem *);

static enum {
	H, D, Q, B, N, W
} CS;


int get_symbol ()
{
	return getchar ();
}


ListElem * new_element (Buffer * buf, TypeLex t_lex)
{
	ListElem * new = malloc (sizeof (ListElem));
	new->string = malloc (buf->cnt);
	strcpy (new->string, buf->string);
	new->lenght = buf->cnt;
	new->t_lex = t_lex; 
	new->next = NULL;

	flush_buffer (buf);

	return new;
}


void put_lex (ListElem * elem, ListElem ** first)
{
	ListElem * cur = *first;
	ListElem * prev;

	if ( cur == NULL ) {
		*first = elem;
	}
	else {
		while ( cur != NULL ) {
			prev = cur;
			cur = cur->next;
		}
		prev->next = elem; 
	}
}


int isdelim (int c)
{
	if ( c == '|' || c == '<' || c == '>' || c == '&') {
		return c;
	}
	
	return 0;
}


TypeLex typify (Buffer * buf)
{
	if ( strcmp (buf->string, "|") == 0 ) {
		return LEX_PIPE;
	}
	else if ( strcmp (buf->string, "&") == 0) {
		return LEX_AMP;
	}
	else if ( strcmp (buf->string, "<") == 0) {
		return LEX_READ;
	}
	else if ( strcmp (buf->string, ">") == 0) {
		return LEX_WRITE;
	}
	else if ( strcmp (buf->string, ">>") == 0) {
		return LEX_APPEND;
	}

	return 0;
}


ListElem * fill_list ()
{
	int c;
	ListElem * elem, * list = NULL;
	Buffer * buf = malloc (sizeof (Buffer));

	init_buffer (buf);

	while ( 1 ) {
		if ( (c = get_symbol ()) == EOF ) {
			break;
		}
		if ( (elem = feed_symbol (c, buf)) != NULL) {
			put_lex (elem, &list);
		}
	}
	if ( (elem = feed_symbol (' ', buf)) != NULL ) {
		put_lex (elem, &list);
	}

	free_buffer (buf);
	free (buf);

	return list;
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
	if ( isspace (c) || c == EOF ) {
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
		return new_element (buf, typify (buf));
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
	if ( isspace (c) || c == EOF ) {
		CS = H;
		return new_element (buf, LEX_WORD);
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
	if ( isspace (c) || c == EOF ) {
		CS = H;
		return new_element (buf, LEX_WORD);
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
		return new_element (buf, LEX_WORD);
	}
	else {
		add_symbol (c, buf);
		return NULL;
	}
}


TreeCmd * parse (ListElem * list)
{
	TreeCmd * cmd = NULL;
	ListElem * cur = list;

	cur = Sp (cur);

	if ( cur == NULL ) {
		printf ("Current ListElem is NULL. OK.\n");
	}

	return cmd;
}


ListElem * Sp (ListElem * cur)
{
	cur = Bp (cur);
	cur = Fp (cur);
	cur = Pp (cur);

	return cur;
}


ListElem * Bp (ListElem * cur)
{
	cur = Cp (cur);

	while ( cur != NULL && cur->t_lex == LEX_PIPE ) {
		cur = Cp (cur);
	}

	return cur;
}


ListElem * Fp (ListElem * cur)
{
	if ( cur == NULL ) {
		printf ("List ended in Fp ().\n");
	}
	else if ( cur->t_lex == LEX_WRITE ) {
		printf (">: YES.\n");
		printf (">>: NO.\n");
		cur = cur->next;
		cur = word (cur);
		cur = Rp (cur);
	}
	else if ( cur->t_lex == LEX_APPEND ) {
		printf (">>: YES.\n");
		printf (">: NO.\n");
		cur = cur->next;
		cur = word (cur);
		cur = Rp (cur);
	}
	else if ( cur->t_lex == LEX_READ ) {
		printf ("<: YES.\n");
		cur = cur->next;
		cur = word (cur);
		cur = Wp (cur);
	}
	else {
		printf ("epsilon-rule in F ().\n");
	}

	return cur;	
}


ListElem * Pp (ListElem * cur)
{
	if ( cur == NULL ) {
		printf ("List ended in Pp ().\n");
	}
	else if ( cur->t_lex == LEX_AMP ) {
		printf ("&: YES.\n");
	}
	else {
		printf ("Unknown lexem. Waiting &.\n");
	}

	return cur;	
}


ListElem * Cp (ListElem * cur)
{
	cur = word (cur);
	cur = Dp (cur);

	return cur;	
}


ListElem * Dp (ListElem * cur)
{
	if ( cur == NULL ) {
		printf ("List ended in Dp ().\n");
	}
	else if (cur->t_lex == LEX_WORD ) {
		printf ("WORD: YES.\n");
		cur = cur->next;
		cur = Dp (cur);
	}
	else {
		printf ("epsilon-rule in D ().\n");
	}

	return cur;	
}
	

ListElem * Rp (ListElem * cur)
{
	if ( cur == NULL ) {
		printf ("List ended in Rp ().\n");
	}
	else if ( cur->t_lex == LEX_READ ) {
		printf ("<: YES.\n");
		cur = cur->next;
		cur = word (cur);
	}
	else {
		printf ("<: NO.\n");
		printf ("epsilon-rule in R ().\n");
	}

	return cur;	
}


ListElem * Wp (ListElem * cur)
{
	if ( cur == NULL ) {
		printf ("List ended in Wp ().\n");
	}
	else if ( cur->t_lex == LEX_WRITE ) {
		printf (">: YES.\n");
		printf (">>: NO.\n");
		cur = cur->next;
		cur = word (cur);
	}
	else if ( cur->t_lex == LEX_APPEND ) {
		printf (">>: YES.\n");
		printf (">: NO.\n");
		cur = cur->next;
		cur = word (cur);
	}
	else {
		printf (">: NO.\n");
		printf (">>: NO.\n");
		printf ("epsilon-rule in W ().\n");
	}

	return cur;	
}


ListElem * word (ListElem * cur)
{
	if ( cur == NULL ) {
		printf ("List ended in word ().\n");
	}
	else if ( cur->t_lex == LEX_WORD ) {
		printf ("Word: YES.\n");
		cur = cur->next;
	}
	else {
		printf ("Error: waiting word.\n");
	}

	return cur;
}
