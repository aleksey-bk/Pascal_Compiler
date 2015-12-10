#include "ExprRealConst.h"


ExprRealConst::ExprRealConst(double n, SymType* t)
{
	type = RealConst;
	expr_type = t;
	real_const = n;
	left = NULL;
	right = NULL;
}


ExprRealConst::~ExprRealConst()
{
}
