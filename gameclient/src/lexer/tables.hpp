#ifndef _TABLES_HPP_
#define _TABLES_HPP_


enum type_of_lex {
	LEX_NULL,				// 0
	LEX_DO,				// 1
	LEX_GOTO,			// 2
	LEX_ELSE,			// 3
	LEX_IF,				// 4
	LEX_THEN,			// 5
	LEX_WHILE,			// 6
	LEX_BEGIN,
	LEX_END,
	LEX_FIN,			// 
	LEX_COMMA,			// 10 
	LEX_LPAREN,			// 11 
	LEX_RPAREN,			// 12
	LEX_EQ,				// 13
	LEX_LESS,			// 14
	LEX_GREATER,			// 15
	LEX_PLUS,			// 16
	LEX_MINUS,			// 17
	LEX_MULTIPLY,			// 18
	LEX_DIVISION, 			// 19
	LEX_LBRACKET,			// 
	LEX_RBRACKET,			// 
	LEX_OR,				// 2
	LEX_AND,			// 2
	LEX_NEQ,			// 27
	LEX_ASSIGN,			// 28
	LEX_NUM,			// 29
	LEX_ID,				// 
	LEX_ARRAY,			// 
	LEX_LABEL,			// 30
	LEX_STR,			// 31
	LEX_BUY,			// 
	LEX_SELL,			// 
	LEX_PROD,			// 
	LEX_BUILD,	
	LEX_TURN,			// 
	LEX_CUR_MONTH,
	LEX_PLAYERS,
	LEX_ACTIVE_PLAYERS,
	LEX_SUPPLY,
	LEX_RAW_PRICE,
	LEX_DEMAND,
	LEX_PRODUCTION_PRICE,
	LEX_MONEY,
	LEX_RAW,
	LEX_PRODUCTION,
	LEX_FACTORIES,
	LEX_AUTO_FACTORIES,
	LEX_MANUFACTURED,
	LEX_RESULT_RAW_SOLD,
	LEX_RESULT_RAW_PRICE,
	LEX_RESULT_PROD_BOUGHT,
	LEX_RESULT_PROD_PRICE,
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

struct PrintTable {
	static const char * 		lexem [];
};

#endif
