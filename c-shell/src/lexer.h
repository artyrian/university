#ifndef _LEXER_H_
#define _LEXER_H_


typedef struct TagListElem {
	char * string;
	int lenght;
	struct TagListElem * next;
} ListElem; 


int fill_list ();


#endif
