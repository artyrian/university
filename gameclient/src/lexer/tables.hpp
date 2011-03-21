#ifndef _TABLES_HPP_
#define _TABLES_HPP_

enum type_of_lex {
	LEX_NULL,				// 0
	LEX_AND,				// 1 
	LEX_BEGIN, 				// 2
	LEX_BOOL,				// 3
	LEX_DO,					// 4
	LEX_ELSE,				// 5
	LEX_END,				// 6
	LEX_IF,					// 7
	LEX_FALSE,				// 8
	LEX_INT,				// 9
	LEX_NOT,				// 10
	LEX_OR,					// 11
	LEX_PROGRAM,				// 12
	LEX_READ,				// 13
	LEX_STR,				// 13.5
	LEX_THEN,				// 14
	LEX_TRUE,				// 15
	LEX_VAR,				// 16
	LEX_WHILE,				// 17
	LEX_WRITE,				// 18
	LEX_FIN,				// 19
	LEX_SEMICOLON, 				// 20
	LEX_COMMA,
	LEX_COLON,
	LEX_ASSIGN,
	LEX_LPAREN,
	LEX_RPAREN,				// 25
	LEX_EQ,
	LEX_LSS,
	LEX_GTR,
	LEX_PLUS,
	LEX_MINUS,				// 30
	LEX_TIMES,
	LEX_SLASH, 
	LEX_LEQ,
	LEX_NEQ,
	LEX_GEQ,				// 35
	LEX_NUM,				// 36
	LEX_ID,					// 37
	LEX_BUY,
	LEX_PROD,
	LEX_SELL,
	LEX_TURN,
	POLIZ_LABEL,				// 
	POLIZ_ADDRESS,				//
	POLIZ_GO,				// 
	POLIZ_FGO				// 
};


class String {
	char * 		name;
	type_of_lex	type;
public:
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
	String & operator [] (int k);
	int put (const char * buf);
	~TableString ();
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
	Ident & operator [] (int k);
	int put (const char * buf);
	~TableIdent ();
};



struct TableLexem {
	static const char *	word [];
	static const char *	delim [];
	static const char *	function [];
	static type_of_lex 	lex_word [];
	static type_of_lex 	lex_delim [];
	static type_of_lex	lex_function [];
	TableString		string;
	TableIdent		ident;

	TableLexem ();
};

#endif
