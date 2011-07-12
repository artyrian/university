#ifndef _LEXER_H_
#define _LEXER_H_

typedef enum TagTypeLex {
	LEX_WORD,
	LEX_AMP,
	LEX_PIPE,
	LEX_READ,
	LEX_WRITE,
	LEX_APPEND
} TypeLex;


typedef struct TagListElem {
	char * string;
	int lenght;
	enum TagTypeLex t_lex;
	struct TagListElem * next;
} ListElem; 


typedef struct TagTreeCmd {
	char ** array;
	TypeLex	t_lex;
	struct TagTreeCmd * next;
} TreeCmd;


ListElem * fill_list ();
TreeCmd * parse (ListElem *);


#endif
