#include "ExprRealConst.h"


ExprRealConst::ExprRealConst(double n)
{
	type = RealConst;
	real_const = n;
	left = NULL;
	right = NULL;
}


ExprRealConst::~ExprRealConst()
{
}
