#include "ExprUnOp.h"


ExprUnOp::ExprUnOp(Expression* left, int t)
{
	type = UnOp;
	this->left = left;
	this->operation = t;
}


ExprUnOp::~ExprUnOp()
{
}
