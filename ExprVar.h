#pragma once
#include "Expression.h"
#include <string>
using namespace std;

class ExprVar :	public Expression
{
public:
	ExprVar(string s);
	~ExprVar();
};

