#pragma once
#include "ExprVar.h"
#include "ExprBinOp.h"

class Statement
{
public:
	enum { CLASS_WHILE, CLASS_ASSIGN, CLASS_BLOCK, CLASS_CALL, CLASS_FOR, CLASS_IF, CLASS_BREAK, CLASS_CONTINUE };
	int class_id;
	Statement(){};
	~Statement(){};
};

