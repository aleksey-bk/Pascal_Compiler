#include "Lexeme.h"



const string Lexeme::LexemeText[10] = { string("string"), string("char"), string("integer"),
string("hex"), string("real"), string("ident"), string("op"), string("sep"), string("keyword"), string("dot_end") };

Lexeme::Lexeme(string s, int t, int r, int c)
{

}

static string FromHexToInt(string hex)
{
	long DecRes = 0, l = hex.length();
	for (int i = 0; i < l; ++i)
	{
		if (hex[i] > '9')
		{
			if (hex[i] > 'F')
				DecRes += ((hex[i] - 'a') + 10) * pow(16, l - i - 1);
			else
				DecRes += ((hex[i] - 'A') + 10) * pow(16, l - i - 1);
		}
		else
			DecRes += (hex[i] - '0') * pow(16, l - i - 1);
	}
	string s = to_string(DecRes);
	return s;
}

void Lexeme::PrintValue(FILE* F)
{
	if (type == TypeInteger)
		fprintf_s(F, "\t%s", text.c_str());
	if (type == TypeReal)
		fprintf_s(F, "\t%.4e", atof(text.c_str()));
	if (type == TypeString)
		fprintf_s(F, "\t%s", text.substr(1, text.length() - 2).c_str());
	if (type == TypeChar)
	{
		if (text[0] == '\'')
			return;
		if (text[1] == '$') //hex
			fprintf_s(F, "\t%c", atoi(FromHexToInt(text.substr(2, text.length() - 2)).c_str()));
		else
			fprintf(F, "\t%c", atoi(text.substr(1, text.length() - 1).c_str()));
	}
	if (type == TypeHex)
		fprintf_s(F, "\t%s", FromHexToInt(text).c_str());
	return;
}

Lexeme::Lexeme()
{
	lexid = -1;
}


Lexeme::~Lexeme()
{

}
