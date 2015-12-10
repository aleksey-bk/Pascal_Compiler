#pragma once
#include <cstdlib>
#include <string>

enum
{
	KWarray, KWbegin, KWbreak, KWcontinue, KWdo, KWdownto, KWelse, KWend, KWfor, KWforward,
	KWfunction, KWif, KWexit, KWof, KWprocedure, KWprogram, KWrecord, KWrepeat, KWthen, KWto, KWtype, KWuntil,
	KWvar, KWwhile, KWin, AND, DIV, MOD, OR, NOT, XOR,
	TypeEOF, TypeErr, TypeString, TypeChar, TypeInteger, TypeHex,
	TypeReal, TypeIdent, TypeOp, TypeSep, TypeKeyword, TypeDotEnd,
	assign, Plus, Minus, Mul, Div, open_br, open_qbr, rec, close_qbr,
	close_br, dog, eq, big, big_or_eq, less_, less_or_eq, not_eq, cap, div_eq, mul_eq, minus_eq, plus_eq,
	com, dot_com, double_dot, dot_dot
};



using namespace std;

class Lexeme
{
public:
	static const string LexemeText[10];
	int row;
	int col;
	int type;
	string text;
	int lexid;
	void PrintValue(FILE *F);
	Lexeme(string s, int t, int r, int c);
	Lexeme();
	~Lexeme();
};

