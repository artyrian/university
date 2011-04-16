#ifndef _TABLES_HPP_
#define _TABLES_HPP_


enum type_of_lex {
	LEX_NULL,			// 0
	LEX_DO,				// 1
	LEX_GOTO,			// 2
	LEX_IF,				// 3
	LEX_THEN,			// 4
	LEX_WHILE,			// 5
	LEX_BEGIN,			// 6
	LEX_END,			// 7
	LEX_FIN,			// 8
	LEX_COMMA,			// 9 
	LEX_LPAREN,			// 10 
	LEX_RPAREN,			// 11
	LEX_EQ,				// 12
	LEX_LESS,			// 13
	LEX_GREATER,			// 14
	LEX_PLUS,			// 15
	LEX_MINUS,			// 16
	LEX_MULTIPLY,			// 17
	LEX_DIVISION, 			// 18
	LEX_LBRACKET,			// 
	LEX_RBRACKET,			// 
	LEX_OR,				// 21
	LEX_AND,			// 22
	LEX_NEG,			// 23
	LEX_ASSIGN,			// 24
	LEX_NUM,			// 25
	LEX_ID,				// 
	LEX_ARRAY,			// 
	LEX_LABEL,			// 28
	LEX_STR,			// 29
	LEX_BUY,			// 30
	LEX_SELL,			// 
	LEX_PROD,			// 
	LEX_BUILD,	
	LEX_TURN,			// 
	LEX_PRINT,
	LEX_CUR_MONTH,
	LEX_PLAYERS,
	LEX_ACTIVE_PLAYERS,
	LEX_SUPPLY,
	LEX_RAW_PRICE,
	LEX_DEMAND,
	LEX_PRODUCTION_PRICE,		// 44
	LEX_MONEY,
	LEX_RAW,
	LEX_PRODUCTION,
	LEX_FACTORIES,
	LEX_AUTO_FACTORIES,
	LEX_MANUFACTURED,		// 50
	LEX_RESULT_RAW_SOLD,
	LEX_RESULT_RAW_PRICE,
	LEX_RESULT_PROD_BOUGHT,
	LEX_RESULT_PROD_PRICE,
	POLIZ_ADDRESS,
	POLIZ_ID_ARRAY,
	POLIZ_GO,
	POLIZ_LABEL,
	POLIZ_FGO,
	POLIZ_NOP
};




class StorageTypeLex {
	char * 		name;
	type_of_lex	type;
	int 		value;
public:
	virtual char * get_name ();
	virtual int * get_address_value ();
	virtual int get_value () const;
	virtual void put_name (const char * str);
	virtual type_of_lex get_type () const;
	virtual void put_type (type_of_lex t);

	virtual ~StorageTypeLex ();
	virtual void operator= (const StorageTypeLex & str);
};




class TableStorageTypeLex : public StorageTypeLex {
	StorageTypeLex * s;
	int		size;
	int		top;

	char * 		name;
	type_of_lex	type;
	int 		value;

	void extend_table ();
public:
	TableStorageTypeLex ();
	StorageTypeLex & operator [] (int k);
	int put (const char * buf);
	int get_size () const;

	virtual ~TableStorageTypeLex ();
	virtual void operator= (const TableStorageTypeLex & t);
	void print () const;
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


class TableLabel {
	struct Item {
		int label;
		int place;
	};
	
	Item * arr;
	int size;
	int top;

	void extend_table ();
public:
	TableLabel ();
	~TableLabel ();
	int get_size ();
	int get_label (int i);
	int get_place (int i);
	int put (int value, int place);
	int look (int val);
	Item operator [] (int k);
	void print () const;
};


struct TableLexem {
	static const char *		word [];
	static type_of_lex 		lex_word [];

	static const char * 		action [];
	static type_of_lex		lex_action [];

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
