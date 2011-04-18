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




struct StorageTypeLex {
	char * 	name;
	type_of_lex type;
	int value;

	virtual void put_name (const char *str);
	virtual int * get_address_value ();
	virtual ~StorageTypeLex ();
};




class TableStorageTypeLex : public StorageTypeLex {
	struct Storage {
		StorageTypeLex * s;
		int num;
		Storage * next;

		Storage (
			StorageTypeLex * p_s, 
			int p_num, 
			Storage * p_next
		);
	};

	Storage *	storage_list;
	int		size;
public:
	TableStorageTypeLex ();
	int put (const char * buf);
//	StorageTypeLex * operator [] (int v);
	StorageTypeLex * index (int idx);

	~TableStorageTypeLex ();
	void print () const;
};




class TableArrayStorageTypeLex {
	struct TableStorage {
		TableStorageTypeLex * 	t;
		int num;
		TableStorage * next;

		TableStorage (
			TableStorageTypeLex * p_t, 
			int p_num, 
			TableStorage * next
		);
	};
	TableStorage * table_storage_list;

	int size;

public:
	TableArrayStorageTypeLex ();

//	TableStorageTypeLex * operator [] (int k);
	TableStorageTypeLex * index (int idx);

	int put (const char * buf);
	~TableArrayStorageTypeLex ();
};


class TableLabel {
	struct Item {
		int label;
		int place;

		Item (int p_label, int p_place);
	};

	struct ListElem {
		Item * item;
		int num;
		ListElem * next;

		ListElem (Item * p_item, int p_num, ListElem * p_next);
	};
	
	ListElem * list;
	int size;
public:
	TableLabel ();
	~TableLabel ();
	int get_size ();
	int put (int value, int place);
	int look (int val);
//	Item * operator [] (int k);
	Item * index (int idx);
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
