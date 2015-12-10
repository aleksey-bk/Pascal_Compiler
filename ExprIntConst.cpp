#include "ExprIntConst.h"


ExprIntConst::ExprIntConst(int n, SymType* t)
{
	type = IntConst;
	expr_type = t;
	int_const = n;
	left = NULL;
	right = NULL;
}


ExprIntConst::~ExprIntConst()
{
}
