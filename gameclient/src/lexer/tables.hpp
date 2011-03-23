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
	LEX_LBRACKET,
	LEX_RBRACKET,
	LEX_NUM,				// 38
	LEX_ID,					// 39
	LEX_ARRAY,
	LEX_BUY,
	LEX_PROD,
	LEX_SELL,
	LEX_TURN,
	POLIZ_LABEL,				// 
	POLIZ_ADDRESS,				//
	POLIZ_GO,				// 
	POLIZ_FGO				// 
};


class StorageTypeLex {
	friend class TableStorageTypeLex;
	char * 		name;
	type_of_lex	type;
	int 		value;
public:
	char * get_name ();
	void put_name (const char * str);
	type_of_lex get_type ();
	void put_type (type_of_lex t);

	virtual ~StorageTypeLex ();
	virtual void operator= (const StorageTypeLex & str);
};


class TableStorageTypeLex: public StorageTypeLex {
	StorageTypeLex *s;
	int		size;
	int		top;

	void extend_table ();
public:
	TableStorageTypeLex ();
	StorageTypeLex & operator [] (int k);
	int put (const char * buf);

	virtual ~TableStorageTypeLex ();

	virtual void operator= (const TableStorageTypeLex & t);

};

class TableArrayStorageTypeLex {
	TableStorageTypeLex * 	t;
	int 			size;
	int 			top;

	void extend_table ();
public:
	TableArrayStorageTypeLex ();
	TableStorageTypeLex & operator [] (int k);
	int put (const char * buf);
	~TableArrayStorageTypeLex ();
};

struct TableLexem {
	static const char *		word [];
	static type_of_lex 		lex_word [];

	static const char *		delim [];
	static type_of_lex 		lex_delim [];

	static const char *		function [];
	static type_of_lex		lex_function [];

	TableStorageTypeLex		string;
	TableStorageTypeLex		ident;

	TableArrayStorageTypeLex	array;
};

#endif
