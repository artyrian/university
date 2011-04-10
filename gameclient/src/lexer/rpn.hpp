#ifndef _RPN_HPP_
#define _RPN_HPP_

#include "lexer.hpp"

/*
template <class T> class Stack {
	int max_size;
	T s [100];
	int top;
public:
	Stack ();
	void reset ();
	void push (T i);
	T pop ();
	bool is_empty ();
	bool is_full ();
};
*/


class PolizElem;

struct PolizItem {
	int number;
	PolizElem * p;
	PolizItem * next;
};


class PolizElem {
public:
	virtual ~PolizElem ();
	virtual void evaluate (	PolizItem ** stack, 
				PolizItem ** cur_cmd) const = 0;
	virtual void print () const {};
protected:
	static void 		push (PolizItem ** stack, PolizElem * elem);
	static PolizElem * 	pop (PolizItem ** stack);
};


class PolizTest: public PolizElem {
	Lex	l;
public:
	virtual void evaluate (	PolizItem ** stack, 
				PolizItem ** cur_cmd) const {};
//	PolizTest (type_of_lex type);
	PolizTest (const Lex & lex);
	void print () const;
};


class PolizConst : public PolizElem  {
public:
	virtual PolizElem * clone () const;
	virtual void evaluate ( PolizItem ** stack, 
				PolizItem ** cur_cmd) const;
};

class PolizInt : public PolizConst {
	int value;
public:
	PolizInt (int a);
	virtual ~PolizInt ();
	virtual PolizElem * clone () const;
	int get () const;
};


class PolizLabel : public PolizConst {
	PolizItem * value;
public:
	PolizLabel (PolizItem * a);
	virtual ~PolizLabel ();
	virtual PolizElem * clone () const;
	PolizItem * get () const;

};


/*
class PolizOpGo : public PolizElem {
public:
	PolizOpGo ();
	virtual ~PolizOpGo ();
	void evaluate ( PolizItem ** stack,
			PolizItem ** cur_cmd) const;
};
*/

//Here class PolizFunction 
/*
class PolizFunPlus : public PolizFunPlus {
public:
	PolizFunPlus ();
	virtual ~PolizFunPlus ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
};
*/

/*
class Poliz {
	int size;
public:
	void put_lex (const Lex & l);
	void put_lex (const Lex & l, int place);

	void blank ();
	int get_size ();

	void print ();
};
*/


class PolizList {
	PolizItem * first;
	PolizItem * create_item (PolizElem * p);
	int size;
public:
	PolizList ();
	~PolizList ();
	void add_to_list (PolizElem * p);
	void add_to_list (PolizElem * p, int place);
	int get_size ();
	void print ();
};


#endif
