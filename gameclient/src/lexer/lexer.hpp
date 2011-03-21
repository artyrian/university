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
	~Ident ();
};

class TableIdent {
	Ident * p;
	int	size;
	int 	top;
	
	void extend_table ();
public:
	TableIdent ();
	Ident & operator[] (int k);
	int put (const char * buf);
	~TableIdent ();
};

class String {
	char * 		name;
	type_of_lex	type;
pulic:
	char * get_name ();
	void put_name (const char * str);
	type_of_lex get_type ();
	void put_type (type_of_lex t);
};

class TableString {
	
	String *s;
	int	size;
	int	top;

	void extend_table ();
public:
	TableString ();
	String & operator[] (int k);
	int put (const char * buf);
	~TableString ();
	

};


class Scanner {
	enum			state 	{	
						H, 
						NUM, 	
						IDENT, 
						KW, 
						ASSIGN, 
						STR, 		// not releazied
						DELIM,
						NEQ, 
						FN		// not releazied
					};

	TableLexem		table;
	TableIdent		table_ident;
	state			CS;
	int			save_char;
	int			count_str;
	Buffer * 		buffer;
	int 			digit;

	int isdelim (int c);
	int look (const char * buf, const char ** list);
public:
	Scanner ();
	Lex feed_symbol (int c);
	~Scanner ();
};

#endif
