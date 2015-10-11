#pragma once
#include <cstdlib>
#include <string>

#define TypeFalse - 2
#define TypeErr -1
#define TypeString 1
#define TypeChar 2
#define TypeInteger 3
#define TypeHex 4
#define TypeReal 5
#define TypeIdent 6
#define TypeOp 7
#define TypeSep 8
#define TypeKeyword 9



using namespace std;

class Lexeme
{
public:
	static const string LexemeText[9];
	int row;
	int col;
	int type;
	string text;
	void PrintValue(FILE *F);
	Lexeme(string s, int t, int r, int c);
	Lexeme();
	~Lexeme();
};

