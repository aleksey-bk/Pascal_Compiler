#include "ExprIntConst.h"


ExprIntConst::ExprIntConst(int n)
{
	type = IntConst;
	intConst = n;
	left = NULL;
	right = NULL;
}


ExprIntConst::~ExprIntConst()
{
}
