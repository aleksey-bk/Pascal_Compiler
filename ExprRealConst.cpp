#include "ExprRealConst.h"


ExprRealConst::ExprRealConst(double n)
{
	type = RealConst;
	realConst = n;
	left = NULL;
	right = NULL;
}


ExprRealConst::~ExprRealConst()
{
}
