#include "ExprVar.h"


ExprVar::ExprVar(string s)
{
	type = Var;
	name = s;
	left = NULL;
	right = NULL;
}


ExprVar::~ExprVar()
{
}
