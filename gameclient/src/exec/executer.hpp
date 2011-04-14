#ifndef _EXECUTER_HPP_
#define _EXECUTER_HPP_

#include "../lexer/parser.hpp"

class Executer {
	PolizItem * poliz_list;
public:
	Executer (PolizItem * list);
	void executing ();
};


#endif
