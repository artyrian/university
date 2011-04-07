#ifndef _POLIZ_HPP_
#define _POLIZ_HPP_

/*
class Stack {
	
	int top;
public:
	Stack ();
	void reset ();
	void push (	);
		pop ();
	bool is_empty ();
	bool is_full ();
};
*/

/*
class PolizItem {
public:

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

class PolizConst  {

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

class PolizOpGo : public PolizElem {
public:
	PolizOpGo ();
	virtual ~PolizOpGo ();
	void evaluate ( PolizItem ** stack,
			PolizItem ** cur_cmd) const;
};

Here class PolizFunction 

class PolizFunPlus : public PolizFunPlus {
public:
	PolizFunPlus ();
	virtual ~PolizFunPlus ();
	PolizElem * evaluate_fun ( PolizItem ** stack) const;
};
*/

class Poliz {

};


#endif
