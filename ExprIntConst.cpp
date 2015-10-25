#include "ExprIntConst.h"


ExprIntConst::ExprIntConst(int n)
{
	type = IntConst;
	int_const = n;
	left = NULL;
	right = NULL;
}


ExprIntConst::~ExprIntConst()
{
}
