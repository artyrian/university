enum type_of_lex
{
		LEX_NULL,
		LEX_AND, LEX_BEGIN/*


		*/
		LEX_FIN
};

Lex::Lex ( type_of_lex t = LEX_NULL, int v = 0)
	:	t_lex = t; v_lex = v;
{
}

type_of_lex Lex::get_type ()
{
	return t_lex;
}

int Lex::get_value ()
{
	return v_lex;
}

void print ()
{
	printf ("(%d,%d)", t_lex, v_lex);

}

Scanner::Scanner ()
	: buf ()
{
	// open where we must read;

	// begin state

	// get first symbol or create new method
	// where I feed symbol;
}

Lex Scanner::get_lex ()
{
	
	return Lex();
}
