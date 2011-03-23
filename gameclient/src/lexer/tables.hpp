#ifndef _TABLES_HPP_
#define _TABLES_HPP_


enum type_of_lex {
	LEX_NULL,				// 0
	LEX_DO,				// 1
	LEX_GOTO			// 2
	LEX_ELSE,			// 3
	LEX_IF,				// 4
	LEX_THEN,			// 5
	LEX_WHILE,			// 6
	LEX_FIN,			// 7
	LEX_COMMA,			// 8
	LEX_LPAREN,			// 9
	LEX_RPAREN,			// 10
	LEX_EQ,				// 11
	LEX_LESS,			// 12
	LEX_GREATER,			// 13
	LEX_PLUS,			// 14
	LEX_MINUS,			// 15
	LEX_MULTIPLY,			// 16
	LEX_DIVISION, 			// 17
	LEX_LEQ,			// 18
	LEX_GEQ,			// 19
	LEX_NEQ,			// 20
	LEX_LBRACKET,			// 21
	LEX_RBRACKET,			// 22
	LEX_BEGIN,			// 23
	LEX_END,			// 24
	LEX_OR,				// 25
	LEX_AND,			// 26
	LEX_NUM,			// 27
	LEX_ID,				// 28
	LEX_ARRAY,			// 29
	LEX_LABEL,			// 30
	LEX_STR,			// 31
	LEX_BUY,			// 32
	LEX_PROD,			// 33
	LEX_SELL,			// 34
	LEX_TURN,			// 35
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
	TableStorageTypeLex		label;

	TableArrayStorageTypeLex	array;
};

#endif
