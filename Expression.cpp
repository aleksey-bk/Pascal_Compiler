#include "Expression.h"


Expression::Expression()
{
	left = NULL;
	right = NULL;
}


Expression::~Expression()
{
	if (left)
		delete left;
	if (right)
		delete right;
}
