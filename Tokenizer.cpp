#define _CRT_SECURE_NO_WARNINGS

#include "Tokenizer.h"

#define SaveChrFalseVal -5
#define ReadCharVal -1

const string Tokenizer::Keywords[LexNum] = { string("array"), string("begin"), string("break"),
string("continue"), string("do"), string("downto"), string("else"), string("end"), string("for"),
string("forward"), string("function"), string("if"), string("in"), string("of"), string("procedure"),
string("program"), string("record"), string("repeat"), string("then"), string("to"), string("type"),
string("until"), string("var"), string("while"), string("exit"), string("and"), string("div"), string("mod"),
string("or"), string("not"), string("xor") };





Tokenizer::Tokenizer(const char* FileName)
{
	fopen_s(&F, FileName, "rt");
	if (F == NULL)
	{
		string mes = string("File not found");
		EOFDetected = true;
		printf_s("%s\n", mes.c_str());
		return;
	}
	row = 1;
	col = 1;
	SaveChr = SaveChrFalseVal;
	EOFDetected = false;
}

Tokenizer::Tokenizer()
{

}


Tokenizer::~Tokenizer()
{

}



int Tokenizer::NextChar()
{
	if (EOFDetected)
		return EOF;
	int r = fgetc(F);
	if (r == EOF)
	{
		EOFDetected = true;
		fclose(F);
	}
	if (r != '\n')
		++col;
	else
	{
		col = 1;
		++row;
	}
	return r;
}

bool Tokenizer::IsDigit(int c)
{
	if (((c - '0') <= 9) && ((c - '0') >= 0))
		return true;
	else
		return false;
}

bool Tokenizer::IsABC(int c)
{
	return ((('Z' - c >= 0) && (c >= 'A')) || (('z' - c >= 0) && (c >= 'a')));
}

bool Tokenizer::IsHexChar(int c)
{
	return (IsDigit(c) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E')
		|| (c == 'F') || (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e')
		|| (c == 'f'));
}

bool Tokenizer::IsQuote(int c)
{
	return (c == '\'');
}

bool Tokenizer::IsDot(int c)
{
	return (c == '.');
}

bool Tokenizer::IsUnderscore(int c)
{
	return (c == '_');
}

bool Tokenizer::IsReal(string s)
{
	int l = s.length();
	for (int i = 0; i < l; ++i)
	{
		if (IsDot(s[i]))
			return true;
	}
	return false;
}

string Tokenizer::ReadHex(Lexeme* l)
{
	int LastChr;
	string hex;
	while (true)
	{
		LastChr = NextChar();
		if (IsHexChar(LastChr))
			hex.push_back(LastChr);
		else
		{
			if (IsABC(LastChr))
			{
				ERR(l, "BadChar");
				return string("");
			}
			if (hex.length() == 0)
			{
				ERR(l, "NoHex");
				return string("");
			}
			SaveChr = LastChr;
			return hex;
		}
	}
}

bool Tokenizer::IsKeyword(string s)
{
	for (int i = 0; i < OpIdx; ++i)
	{
		if (s == Keywords[i])
			return true;
	}
	return false;
}

bool Tokenizer::IsOp(string s)
{
	for (int i = OpIdx; i < LexNum; ++i)
	{
		if (s == Keywords[i])
			return true;
	}
	return false;
}

string Tokenizer::ReadNumber(int PrevChr, Lexeme* l)
{
	bool dot = false;
	string n;	
	int LastChr;
	if (PrevChr != -1)      //read char number
		n.push_back(PrevChr); 
	while (true)
	{
		LastChr = NextChar();
		if (IsDot(LastChr))
		{
			if (IsDot(n[n.length() - 1]))
			{
				n.pop_back();
				fseek(F, -2, SEEK_CUR); //..
				return n;
			}
			if (dot == true)
			{
				ERR(l, "BadNumber");
				return string("");
			}
			dot = true;
		}
		if (EOFDetected)
			return n;
		if (!IsDigit(LastChr) && !IsDot(LastChr))
		{
			if (IsABC(LastChr))
			{
				ERR(l, "BadChar");
				return string("");
			}
			if (IsDot(n[n.length() - 1]))
			{
				ERR(l, "NoFract");
				return string("");
			}
			SaveChr = LastChr;
			return n;
		};
		n.push_back(LastChr);
	}
}

string Tokenizer::ReadWord(int PrevChr)
{
	string word;
	word.push_back(PrevChr);
	int LastChr;
	while (true)
	{
		LastChr = NextChar();
		if (EOFDetected)
			return word;
		if (!IsDigit(LastChr) && !IsABC(LastChr) && !IsUnderscore(LastChr))
		{
			SaveChr = LastChr;
			return word;
		}
		word.push_back(LastChr);
	}
}

string Tokenizer::ReadString(Lexeme* l)
{
	string str;
	int LastChr;
	while (true)
	{
		LastChr = NextChar();
		if (LastChr == '\n')
		{
			ERR(l, "BadNL");
			return string("");
		}
		if (EOFDetected)
		{
			ERR(l, "BadEOF");
			return string("");
		}
		if (IsQuote(LastChr))
		{
			LastChr = NextChar();
			if (IsQuote(LastChr))
			{
				str.push_back('\'');
				continue;
			}
			return str;
		}
		str.push_back(LastChr);
	}
}

int Tokenizer::StrToInt(string s)
{
	return atoi(s.c_str());
}

void Tokenizer::Read1LineComment()
{
	int NextChr;
	while (true)
	{
		NextChr = NextChar();
		if ((NextChr == '\n') || (NextChr == EOF))
			return;
	}
	return;
}

void Tokenizer::ReadComments(Lexeme* l)
{
	int NextChr;
	while (true)
	{
		NextChr = NextChar();
		if (EOFDetected)
		{
			ERR(l, "BadEOF");
			return;
		}
		if (NextChr == '}')
			return;
	}
	return;
}

void Tokenizer::ERR(Lexeme* l, const char* mes)
{
	l->text = string(mes);
	l->col = col - 1;
	l->row = row;
	l->type = -1;
	EOFDetected = true;
	fclose(F);
	return;
}

Lexeme Tokenizer::NextLex()
{
	Lexeme ret;
	if (EOFDetected)
	{
		ret.type = TypeFalse;
		ret.text = "EOF";
		ret.row = row;
		ret.col = col;
		return ret;
	}
	int Lex = 0, NextChr;
	while (true)
	{
		if (SaveChr != SaveChrFalseVal)
			NextChr = SaveChr;
		else
			NextChr = NextChar();
		SaveChr = SaveChrFalseVal;
		if (NextChr == EOF)
		{
			ret.type = TypeFalse;  
			return ret;
		}
		if (IsDigit(NextChr))
		{
			ret.row = row;
			ret.col = col - 1;
			ret.text += ReadNumber(NextChr, &ret);
			if (ret.type == TypeErr)
				return ret;
			if (IsReal(ret.text))
				ret.type = TypeReal;
			else
				ret.type = TypeInteger;
			break;
		}
		if (IsABC(NextChr))
		{
			ret.row = row;
			ret.col = col - 1;
			ret.text += ReadWord(NextChr);
			if (ret.type == TypeErr)
				return ret;
			if (IsKeyword(ret.text))
				ret.type = TypeKeyword;
			else
			{
				if (IsOp(ret.text))
					ret.type = TypeOp;
				else
					ret.type = TypeIdent;
			}
			break;
		}
		if (IsQuote(NextChr))
		{
			ret.row = row;
			ret.col = col - 1;
			ret.text.push_back('\'');
			ret.text += ReadString(&ret);
			if (ret.type == TypeErr)
				return ret;
			ret.text.push_back('\'');
			if (ret.text.length() > 3)
				ret.type = TypeString;
			else
				ret.type = TypeChar;
			break;
		}
		switch (NextChr)
		{
			case ' ':
			{
				continue;
			}
			case '\n':
			{
				continue;
			}
			case ':':
			{
				ret.row = row;
				if ((NextChr = NextChar()) == '=')
				{
					ret.text.push_back(':');
					ret.text.push_back('=');
					ret.col = col - 2;
					ret.type = TypeOp;
					break;
				}
				else
				{
					ret.col = col - 2;
					SaveChr = NextChr;
					ret.text.push_back(':');
					ret.type = TypeSep;
					break;
				}
			}
			case '$':
			{
				ret.col = col - 1;
				ret.row = row;
				ret.text = ReadHex(&ret);
				ret.type = TypeHex;
				break;
			}
			case '#':
			{
				ret.col = col - 1;
				ret.row = row;
				ret.text.push_back('#');
				if ((NextChr = NextChar()) == '$')
				{
					ret.text.push_back('$');
					ret.text += ReadHex(&ret);
					if (ret.type == TypeErr)
						return ret;
				}
				else
				{
					if (!IsDigit(NextChr))
					{
						ERR(&ret, "NoCC");
						return ret;
					}
					ret.text += ReadNumber(NextChr, &ret);
				}
				if ((atoi(ret.text.c_str()) == EOF))
				{
					ERR(&ret, "BadEOF");
					return ret;
				}
				if (atoi(ret.text.c_str()) > 127)
				{
					ERR(&ret, "BadCC");
					return ret;
				}
				ret.type = TypeChar;
				break;
			}
			case '+':
			{
				ret.text.push_back('+');
				if ((NextChr = NextChar()) == '=')
					ret.text.push_back('=');
				else
					SaveChr = NextChr;
				ret.type = TypeOp;
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '-':
			{
				ret.text.push_back('-');
				if ((NextChr = NextChar()) == '=')
					ret.text.push_back('=');
				else
					SaveChr = NextChr;
				ret.type = TypeOp;
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '*':
			{
				ret.text.push_back('*');
				if ((NextChr = NextChar()) == '=')
					ret.text.push_back('=');
				else
					SaveChr = NextChr;
				ret.type = TypeOp;
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '/':
			{
				NextChr = NextChar();
				if (NextChr == '/')
				{
					Read1LineComment();
					continue;
				}
				ret.text.push_back('/');
				if ((NextChr) == '=')
					ret.text.push_back('=');
				else
					SaveChr = NextChr;
				ret.type = TypeOp;
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '^':
			{
				ret.text.push_back('^');
				ret.type = TypeOp;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case '<':
			{
				ret.text.push_back('<');
				NextChr = NextChar();
				if ((NextChr == '=') || (NextChr == '>'))
					ret.text.push_back(NextChr);
				else
					SaveChr = NextChr;
				ret.type = TypeOp;
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '>':
			{
				ret.text.push_back('>');
				if ((NextChr = NextChar()) == '=')
					ret.text.push_back('=');
				else
					SaveChr = NextChr;
				ret.type = TypeOp;
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '=':
			{
				ret.text.push_back('=');
				ret.type = TypeOp;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case '@':
			{
				ret.text.push_back('@');
				ret.type = TypeOp;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case '(':
			{
				ret.text.push_back(NextChr);
				ret.type = TypeSep;
				ret.col = col - 1;
				ret.row = row; 
				break;
			}
			case ')':
			{
				ret.text.push_back(NextChr);
				ret.type = TypeSep;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case '[':
			{
				ret.text.push_back(NextChr);
				ret.type = TypeSep;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case ']':
			{
				ret.text.push_back(NextChr);
				ret.type = TypeSep;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case ';':
			{
				ret.text.push_back(NextChr);
				ret.type = TypeSep;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case ',':
			{
				ret.text.push_back(NextChr);
				ret.type = TypeSep;
				ret.col = col - 1;
				ret.row = row;
				break;
			}
			case '.':
			{
				ret.text.push_back('.');
				NextChr = NextChar();
				if (NextChr == '.')
				{
					ret.text.push_back('.');
					ret.type = TypeSep;
				}
				else
				{
					SaveChr = NextChr;
					ret.type = TypeOp;
				}
				ret.col = col - 2;
				ret.row = row;
				break;
			}
			case '{':
			{
				ReadComments(&ret);
				if (ret.type == -1)
					return ret;
				continue;
			}
			default:
			{
				ret.col = col - 1;
				ret.row = row;
				ERR(&ret, "Unknown symbol");
			}
		}
		break;
	}
	return ret;
}


bool Tokenizer::End()
{
	return EOFDetected;
}


