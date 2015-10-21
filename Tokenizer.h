#pragma once
#include "LexemeList.h"
#include <cstdlib>
#include "Lexeme.h"
#define LexNum 31
#define OpIdx 25
class Tokenizer
{
	friend class Parser;
private:
	FILE* F;
	LexemeList List;
	int SaveChr;
	int row;
	int col;
	bool EOFDetected;
	static const string Keywords[LexNum];
	int NextChar();
	bool IsDigit(int c);
	bool IsABC(int c);
	bool IsQuote(int c);
	bool IsDot(int c);
	bool IsUnderscore(int c);
	bool IsKeyword(string s, int* lexid);
	bool IsHexChar(int c);
	bool IsOp(string s, int* lexid);
	bool IsReal(string s);
	void Read1LineComment();
	void ReadComments(Lexeme* l);
	void ErrLexeme(Lexeme* l);
	void FalseLexeme(Lexeme *l);
	void ERR(Lexeme* l, const char* mes);
	int StrToInt(string s);
	string ReadHex(Lexeme* l);
	string ReadNumber(int PrevChr, Lexeme* l);
	string ReadWord(int PrevChr);
	string ReadString(Lexeme* l);
public:
	Tokenizer(const char* file);
	Tokenizer();
	Lexeme NextLex();
	bool End();
	~Tokenizer();
};

