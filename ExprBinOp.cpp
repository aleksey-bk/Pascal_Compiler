#include "ExprBinOp.h"


ExprBinOp::ExprBinOp(Expression* L, int t, Expression* R)
{
	type = BinOp;
	left = L;
	right = R;
	operation = t;
}


ExprBinOp::~ExprBinOp()
{

}
