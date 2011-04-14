#ifndef _RPN_HPP_
#define _RPN_HPP_

#include "lexer.hpp"

class PolizElem;
struct PolizItem;

//----------------------------------------------------------

class Stack {
	PolizItem * first;
	int size;

	PolizItem * create_item (PolizElem * cur_cmd);
public:
	Stack ();
	~ Stack ();
	void push (PolizElem * cur_cmd);
	PolizElem * pop ();
};

//----------------------------------------------------------

struct PolizItem {
	int number;
	PolizElem * p;
	PolizItem * next;

	PolizItem (int, PolizElem *, PolizItem *);
};

//----------------------------------------------------------

class PolizElem {
public:
	virtual ~PolizElem ();
	virtual void evaluate (	PolizItem ** stack, 
				PolizItem ** cur_cmd) const = 0;
	virtual void print () const;
protected:
	static void push (PolizItem ** stack, PolizElem * elem);
	static PolizElem * pop (PolizItem ** stack);
};

//----------------------------------------------------------

class PolizNop : public PolizElem {
public:
	virtual void evaluate (	PolizItem ** stack, 
				PolizItem ** cur_cmd) const;
	void print () const;
};

//----------------------------------------------------------

class PolizConst : public PolizElem  {
public:
	virtual PolizElem * clone () const;
	virtual void evaluate ( PolizItem ** stack, 
				PolizItem ** cur_cmd) const;
};

//----------------------------------------------------------

class PolizInt : public PolizConst {
	int value;
public:
	PolizInt (int a);
	virtual PolizElem * clone () const;
	int get () const;
	void print () const;
};


class PolizString : public PolizConst {
	char * value;
public:
	PolizString (char * value);
	virtual ~PolizString ();
	virtual PolizElem * clone () const;
	char * get () const;
	void print () const;
};


class PolizVarAddress : public PolizConst {
	int * value;
public:
	PolizVarAddress (int * value);
	virtual PolizElem * clone () const;
	int * get () const;
	void print () const;
};


class PolizLabel : public PolizConst {
	PolizItem * value;
public:
	PolizLabel (PolizItem * a);
	virtual ~PolizLabel ();
	virtual PolizElem * clone () const;
	PolizItem * get () const;
	void print () const;
};

//----------------------------------------------------------

class PolizOpGo : public PolizElem {
public:
	PolizOpGo ();
	virtual ~PolizOpGo ();
	void evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const;
	void print () const;
};

//----------------------------------------------------------

class PolizOpGoFalse : public PolizElem {
public:
	PolizOpGoFalse ();
	virtual ~PolizOpGoFalse ();
	void evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const;
	void print () const;
};

//----------------------------------------------------------

class PolizFunction : public PolizElem {

public:
	virtual PolizElem *
		evaluate_fun (PolizItem ** stack) const = 0; 
	void evaluate ( PolizItem ** stack,
			PolizItem ** cur_cmd) const;
	virtual ~PolizFunction ();
};

//----------------------------------------------------------

class PolizArray : public PolizFunction {
	int array;
	TableLexem * table;
public:
	PolizArray (int arr, TableLexem * p_table);
	PolizElem * evaluate_fun ( PolizItem ** stack ) const;
	void print () const;
};


class PolizVarAddressArray : public PolizFunction {
	int array;
	TableLexem * table;
public:
	PolizVarAddressArray (int arr, TableLexem * p_table);
	PolizElem * evaluate_fun ( PolizItem ** stack ) const;
	void print () const;
};


class PolizAssign : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack ) const;
	void print () const;
};


class PolizFunEq : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunGreater: public PolizFunction {
public:
//	PolizFunGreater ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunLess: public PolizFunction {
public:
//	PolizFunLess ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};



class PolizFunPlus : public PolizFunction {
public:
//	PolizFunPlus ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunMinus: public PolizFunction {
public:
//	PolizFunMinus ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunOr: public PolizFunction {
public:
//	PolizFunOr ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunMul: public PolizFunction {
public:
//	PolizFunMul ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};



class PolizFunDiv: public PolizFunction {
public:
//	PolizFunDiv ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunAnd: public PolizFunction {
public:
//	PolizFunAnd ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};

class PolizFunNeg: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};

//----------------------------------------------------------

class PolizFunBuy: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunSell: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunProd: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunBuild: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunTurn: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunPrint: public PolizFunction {
	type_of_lex type;
public:
	PolizFunPrint (type_of_lex type);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};

//----------------------------------------------------------

class PolizFunCurMonth : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunPlayers : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunActivePlayers : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunSupply : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunRawPrice: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunDemand: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunProductionPrice: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunMoney : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunRaw : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunProduction : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunFactories : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunAutoFactories : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunManufactured : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultRawSold : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultRawPrice : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultProdBought : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultProdPrice : public PolizFunction {
public:
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};

//----------------------------------------------------------

class PolizTest: public PolizElem {
	Lex	l;
public:
	virtual void evaluate (	PolizItem ** stack, 
				PolizItem ** cur_cmd) const;
	PolizTest (const Lex & lex);
	void print () const;
};

//----------------------------------------------------------

class PolizList {
	PolizItem * first;
	int size;
	Stack st;

	PolizItem * create_item (PolizElem * p);
public:
	PolizList ();
	~PolizList ();
	void add_to_list (PolizElem * p);
	void add_to_list (PolizElem * p, int place);
	int get_size () const;
	PolizItem * get_pointer (int place) const;
	void print () const;
};


#endif
