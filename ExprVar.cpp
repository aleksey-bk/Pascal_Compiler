#include "ExprVar.h"


ExprVar::ExprVar(string s)
{
	type = Ident;
	name = s;
	left = NULL;
	right = NULL;
}


ExprVar::~ExprVar()
{
}
