#include "ExprBinOp.h"


ExprBinOp::ExprBinOp(Expression* L, int t, Expression* R)
{
	left = L;
	right = R;
	type = t;
}


ExprBinOp::~ExprBinOp()
{

}
