#include "ExprUnOp.h"


ExprUnOp::ExprUnOp(Expression* left, int t, Expression* right)
{
	this->left = left;
	this->right = right;
	this->type = t;
}


ExprUnOp::~ExprUnOp()
{
}
