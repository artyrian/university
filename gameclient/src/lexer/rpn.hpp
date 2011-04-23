#ifndef _RPN_HPP_
#define _RPN_HPP_

#include "tables.hpp"
#include "../robot/game.hpp"

class PolizElem;
struct PolizItem;


struct PolizItem {
	int number;
	PolizElem * p;
	PolizItem * next;

	PolizItem (int = 0, PolizElem * = 0, PolizItem * = 0);
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
	long long int value;
public:
	PolizInt (long long int a);
	virtual PolizElem * clone () const;
	long long int get () const;
	void print () const;
};

class PolizVarInt : public PolizConst {
	long long int * value;
public:
	PolizVarInt (long long int * a);
	virtual PolizElem * clone () const;
	long long int get () const;
	void print () const;
};



class PolizString : public PolizConst {
	char * value;
public:
	PolizString (char * value);
	virtual PolizElem * clone () const;
	char * get () const;
	void print () const;
};


class PolizVarAddress : public PolizConst {
	long long int * value;
public:
	PolizVarAddress (long long int * value);
	virtual PolizElem * clone () const;
	long long int * get () const;
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
	void evaluate ( PolizItem ** stack, PolizItem ** cur_cmd) const;
	void print () const;
};

//----------------------------------------------------------

class PolizOpGoFalse : public PolizElem {
public:
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
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunLess: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunGreaterEq: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunLessEq: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunLessGreater: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};



class PolizFunPlus : public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunMinus: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunOr: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunMul: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};



class PolizFunDiv: public PolizFunction {
public:
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunAnd: public PolizFunction {
public:
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
	Game * robot;
public:
	PolizFunBuy (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunSell: public PolizFunction {
	Game * robot;
public:
	PolizFunSell (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunProd: public PolizFunction {
	Game * robot;
public:
	PolizFunProd (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunBuild: public PolizFunction {
	Game * robot;
public:
	PolizFunBuild (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunTurn: public PolizFunction {
	Game * robot;
public:
	PolizFunTurn (Game * robot);
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

class PolizFunMyId : public PolizFunction {
	Game * robot;
public:
	PolizFunMyId (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunCurMonth : public PolizFunction {
	Game * robot;
public:
	PolizFunCurMonth (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunPlayers : public PolizFunction {
	Game * robot;
public:
	PolizFunPlayers (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunActivePlayers : public PolizFunction {
	Game * robot;
public:
	PolizFunActivePlayers (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunSupply : public PolizFunction {
	Game * robot;
public:
	PolizFunSupply (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunRawPrice: public PolizFunction {
	Game * robot;
public:
	PolizFunRawPrice (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunDemand: public PolizFunction {
	Game * robot;
public:
	PolizFunDemand (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunProductionPrice: public PolizFunction {
	Game * robot;
public:
	PolizFunProductionPrice (Game * robot);
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
	void print () const;
};


class PolizFunMoney : public PolizFunction {
	Game * robot;
public:
	PolizFunMoney (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunRaw : public PolizFunction {
	Game * robot;
public:
	PolizFunRaw (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunProduction : public PolizFunction {
	Game * robot;
public:
	PolizFunProduction (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunFactories : public PolizFunction {
	Game * robot;
public:
	PolizFunFactories (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunAutoFactories : public PolizFunction {
	Game * robot;
public:
	PolizFunAutoFactories (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunManufactured : public PolizFunction {
	Game * robot;
public:
	PolizFunManufactured (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultRawSold : public PolizFunction {
	Game * robot;
public:
	PolizFunResultRawSold (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultRawPrice : public PolizFunction {
	Game * robot;
public:
	PolizFunResultRawPrice (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultProdBought : public PolizFunction {
	Game * robot;
public:
	PolizFunResultProdBought (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};


class PolizFunResultProdPrice : public PolizFunction {
	Game * robot;
public:
	PolizFunResultProdPrice (Game * robot);
	PolizElem * evaluate_fun (PolizItem ** stack) const;
	void print () const;
};

//----------------------------------------------------------

class PolizList {
	PolizItem * first;
	int size;

	PolizItem * create_item (PolizElem * p);
public:
	PolizList ();
	~PolizList ();
	void add_to_list (PolizElem * p);
	void add_to_list (PolizElem * p, int place);
	int get_size () const;
	PolizItem * get_pointer (int place = 1) const;
	void print () const;
};


#endif
