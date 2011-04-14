#ifndef _EXECUTER_HPP_
#define _EXECUTER_HPP_

#include "../lexer/parser.hpp"
#include "../lexer/rpn.hpp"


class Executer {
	PolizItem * poliz_list;
	PolizItem * current;


	PolizItem * stack;
	void push (PolizElem * cur_cmd);
	PolizElem * pop ();
public:
	Executer (PolizItem * list);
	~Executer ();
	void executing ();
};


#endif
