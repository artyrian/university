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
	PolizElem * p;
	PolizItem * next;
};


class PolizElem {
public:
	virtual ~PolizElem ();
	virtual void evaluate (	PolizItem ** stack, 
				PolizItem ** cur_cmd) const = 0;
protected:
	static void 		push (PolizItem ** stack, PolizElem * elem);
	static PolizElem * 	pop (PolizItem ** stack);
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

class Poliz {
	struct PolizElem {
		Lex *		lex;
		int 		number;
		PolizElem *	next;
	};
	PolizElem *	first;
	PolizElem *	last;

	PolizElem * create_elem (const Lex & l);
	int size;
public:
	Poliz ();
	~Poliz ();

	void put_lex (const Lex & l);
	void put_lex (const Lex & l, int place);

	void blank ();
	int get_size ();

	void print ();
};

#endif