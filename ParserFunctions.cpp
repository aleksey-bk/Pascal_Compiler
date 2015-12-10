#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"


Parser::Parser(const char* FileName) : T(FileName)
{
	root = NULL;
	LoopCounter = 0;
}

void Parser::Parse()
{
	Lexeme lex = NextLex();
	LexBuffer.push(lex);
	if (IsSectionName(lex))
		ParseDeclaration();
	stmt_root = ParseStmt(false, true);
}

void Parser::Print(string FileName)
{
	ofstream s(FileName, ios_base::trunc);
	s.close();
	PrintDecl(FileName);
	PrintStmt(FileName);
	return;
}

Lexeme Parser::NextLex()
{
	if (LexBuffer.size() > 0)
	{
		Lexeme ret = LexBuffer.front();
		LexBuffer.pop();
		return LastLex = ret;
	}
	return LastLex = T.NextLex();
}

exception Parser::GetErrInformation(Lexeme l, string inf)
{
	char buff[20];
	exception e((string(_itoa(l.row, buff, 10)) + string(" ") +
		string(_itoa(l.col, buff, 10)) + string(" err: ") + string("\"" + l.text + "\"") +
		string(" ") + inf).c_str());
	return e;
}