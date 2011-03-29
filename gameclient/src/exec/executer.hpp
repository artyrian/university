#ifndef _EXECUTER_HPP_
#define _EXECUTER_HPP_

#include "../lexer/lexer.hpp"
#include "../lexer/poliz.hpp"
#include "../lexer/parser.hpp"


class Executer {
	Lex poliz_el;
public:
	void execute (Poliz & prg);
};


class Interpretator {
	Parser pars;
	Executer exec;
public:
	Interpretator (char * path);
	void interpretation ();
};


#endif
