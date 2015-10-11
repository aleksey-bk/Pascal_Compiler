#pragma once
#include <cstdio>
#include "Lexeme.h"

class LexemeList
{
private:
	struct ListElem
	{
		ListElem *next;
		Lexeme L;
	};
	ListElem *S;
	ListElem *Last;
public:
	LexemeList();
	~LexemeList();
	void Add(Lexeme in);
	void PrintAll(FILE* F);
};

