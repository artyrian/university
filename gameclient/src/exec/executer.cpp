#include "executer.hpp"

void Executer:: execute (Poliz & prg)
{

}




Interpretator:: Interpretator (char * path)
	: pars (path)
{
}


void Interpretator:: interpretation()
{
	pars.analyze ();

	exec.execute (pars.prg);
}
