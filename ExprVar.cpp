#include "ExprVar.h"


ExprVar::ExprVar(string s, SymType* t)
{
	type = Var;
	name = s;
	expr_type = t;
	left = NULL;
	right = NULL;
}


ExprVar::~ExprVar()
{
}
