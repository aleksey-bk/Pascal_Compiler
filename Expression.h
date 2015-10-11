#pragma once
#include <cstdlib>
#include <string>
#define Var -3
#define RealConst -2
#define IntConst -1
#define Add 1
#define Sub 2
#define Mul 3
#define Div 4

using namespace std;

class Expression
{
public:
	int type;
	Expression* right;
	Expression* left;
	int intConst;
	double realConst;
	string name;
	Expression();
	~Expression();
};

