#include "ExprCharConst.h"


ExprCharConst::ExprCharConst(char c, SymType* t)
{
	type = CharConst;
	expr_type = t;
	char_const = c;
	left = NULL;
	right = NULL;
}


ExprCharConst::~ExprCharConst()
{
}
