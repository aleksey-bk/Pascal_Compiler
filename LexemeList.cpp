#define _CRT_SECURE_NO_WARNINGS
#include "LexemeList.h"
#include "keywords.cpp"
#include <cstdlib>

LexemeList::LexemeList()
{
	S = NULL;
	Last = NULL;
}


LexemeList::~LexemeList()
{
	while (S != NULL)
	{
		ListElem* S1 = S->next;
		free(S);
		S = S1;
	}
}


void LexemeList::Add(Lexeme in)
{
	if (in.type == TypeEOF)
		return;
	if (S == NULL)
	{
		S = (ListElem*)calloc(1, sizeof(ListElem));
		S->L = in;
		S->next = NULL;
		Last = S;
	}
	else
	{
		ListElem* a = (ListElem*)calloc(1, sizeof(ListElem));
		Last->next = a;
		a->L = in;
		a->next = NULL;
		Last = a;
	}
	return;
}


void LexemeList::PrintAll(FILE* F)
{
	if (S == NULL)
		return;
	int c;
	ListElem* cur = S;
	if (F == stdout)
		fputc('\n', F);
	while (true)
	{
		if (cur->L.type == TypeEOF)
		{
			fprintf_s(F, "%d\t%d\t%s", cur->L.row, cur->L.col, cur->L.text.c_str());
			return;
		}
		fprintf_s(F, "%d\t%d\t%s\t%s", cur->L.row, cur->L.col,
			Lexeme::LexemeText[cur->L.type - 1].c_str(), cur->L.text.c_str()); //false index dont work
		if ((cur->L.type == TypeInteger) || (cur->L.type == TypeReal)
			|| (cur->L.type == TypeString) || (cur->L.type == TypeChar) 
			|| (cur->L.type == TypeHex))
			cur->L.PrintValue(F);
		if (cur->next != NULL)
			fputc('\n', F);
		else
		{
			if (F == stdout)
				fputc('\n', F);
			break;
		}
		cur = cur->next;
	}
	return;
}
