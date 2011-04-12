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
	virtual ~PolizInt ();
	virtual PolizElem * clone () const;
	int get () const;
	void print () const;
};

//----------------------------------------------------------

class PolizString : public PolizConst {

};
//----------------------------------------------------------

class PolizVarAddress : public PolizConst {

};
//----------------------------------------------------------

class PolizLabel : public PolizConst {
	PolizItem * value;
public:
	PolizLabel (PolizItem * a);
	virtual ~PolizLabel ();
	virtual PolizElem * clone () const;
	PolizItem * get () const;

};

//----------------------------------------------------------

class PolizOpGo : public PolizElem {
public:
	PolizOpGo ();
	virtual ~PolizOpGo ();
	void evaluate ( PolizItem ** stack,
			PolizItem ** cur_cmd) const;
};

//----------------------------------------------------------

class PolizOpGoFalse : public PolizElem {

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

class PolizFunEq: public PolizFunction {
public:
//	PolizFunEq ();
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
////
////
////
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
	int get_size ();
	void print ();
};


#endif
