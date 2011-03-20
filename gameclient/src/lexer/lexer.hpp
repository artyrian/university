#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include "../buffer/buffer.hpp"
#include "tables.hpp"

class Lex {
	type_of_lex	t_lex;
	int 		v_lex;
public:
	Lex (type_of_lex t, int v);
	type_of_lex get_type ();
	int get_value ();
	void print (); 
};

class Ident {
	char * 		name;
	bool		declare;
	type_of_lex	type;
	bool		assign;
	int 		value;
public:
	Ident ();
	char * get_name ();
	void put_name (const char * str);
	bool get_declare ();
	void put_declare ();
	type_of_lex get_type ();
	void put_type ( type_of_lex t);
	bool get_assign ();
	void put_assign ();
	int get_value ();
	void put_value (int v);
};

class TableIdent {
	Ident * p;
	int	size;
	int 	top;
public:
	TableIdent (int max_size);
	Ident & operator[] (int k);
	int put (const char * buf);
	~TableIdent ();
};



class Scanner {
	enum			state { H, NUM, IDENT, KW, AS, STR };
	TableLexem		table;
	state			CS;
	int			c;
	Buffer * 		buf;

	int look (const char * buf, char ** list);
public:
	Scanner ();
	Lex feed_symbol (int c);
};

#endif
