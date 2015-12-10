#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Parser.h"


void TLaunch()
{
	while (true)
	{
		string FileName;
		cin >> FileName;
		if (FileName == "exit")
			break;
		Tokenizer T(FileName.c_str());
		if (T.End())
			continue;
		LexemeList L;
		while (!(T.End()))
			L.Add(T.NextLex());
		FILE* F;
		string OutName = string("OUTPUT") + FileName;
		fopen_s(&F, OutName.c_str(), "wt");
		L.PrintAll(F);
		printf("%s\n", string("result in \"" + OutName + "\"").c_str());
		fclose(F);
	}
	return;
}

void TTestLaunch()
{
	return;
}

void PExprLaunch()
{
	while (true)
	{
		string FileName;
		cin >> FileName;
		if (FileName == "exit")
			break;
		else
		{
			try
			{
				Parser P(FileName.c_str());
				P.ParserExprTEST();
				P.PrintExprTree(FileName.substr(0, FileName.length() - 3) + "out", true);
				//printf("%lf\n", P.CalcTree());
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				continue;
			}
		}
	}
	return;
}

void PExprLaunch_NOT_TREE()
{
	while (true)
	{
		string FileName;
		cin >> FileName;
		if (FileName == "exit")
			break;
		else
		{
			try
			{
				Parser P(FileName.c_str());
				P.ParserExprTEST();
				P.PrintExprMain(FileName.substr(0, FileName.length() - 3) + "out");
				//printf("%lf\n", P.CalcTree());
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				continue;
			}
		}
	}
	return;
}

void PDeclLaunch()
{
	while (true)
	{
		string FileName;
		cin >> FileName;
		if (FileName == "exit")
			break;
		else
		{
			try
			{
				Parser P(FileName.c_str());
				P.ParserDeclTEST();
				P.PrintDecl(FileName.substr(0, FileName.length() - 3) + "out", true);
				//printf("%lf\n", P.CalcTree());
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				continue;
			}
		}
	}
	return;
}

void PExprTestLaunch()
{
	char* sourceTest = "sourceTest.txt";
	ifstream FS(sourceTest);
	string FN;
	while (true)
	{
		if (FS.eof())
			break;
		getline(FS, FN);
		try
		{
			Parser P(FN.c_str());
			P.ParserExprTEST();
			P.PrintExprTree(FN.substr(0, FN.length() - 3) + "out", true);
			//printf("%lf\n", P.CalcTree());
		}
		catch (exception a)
		{
			printf_s("\n%s\n\n", a.what());
			continue;
		}
	}
	return;
}

void PDeclTestLaunch()
{
	char* sourceTest = "sourceTest.txt";
	ifstream FS(sourceTest);
	string FN;
	while (true)
	{
		if (FS.eof())
			break;
		getline(FS, FN);
		try
		{
			Parser P(FN.c_str());
			P.ParserDeclTEST();
			P.PrintDecl(FN.substr(0, FN.length() - 3) + "out", true);
		}
		catch (exception a)
		{
			printf_s("\n%s\n\n", a.what());
			continue;
		}
	}
	return;
}

void PStmtTestLaunch()
{
	char* sourceTest = "sourceTest.txt";
	ifstream FS(sourceTest);
	string FN;
	while (true)
	{
		if (FS.eof())
			break;
		getline(FS, FN);
		try
		{
			Parser P(FN.c_str());
			P.ParserStmtTEST();
			P.PrintStmt(FN.substr(0, FN.length() - 3) + "out", true);
		}
		catch (exception a)
		{
			printf_s("\n%s\n\n", a.what());
			continue;
		}
	}
	return;
}

void PStmtLaunch()
{
	while (true)
	{
		string FileName;
		cin >> FileName;
		if (FileName == "exit")
			break;
		else
		{
			try
			{
				Parser P(FileName.c_str());
				P.ParserStmtTEST();
				P.PrintStmt(FileName.substr(0, FileName.length() - 3) + "out", true);
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				continue;
			}
		}
	}
	return;
}


void ParserLaunch()
{
	while (true)
	{
		string FileName;
		cin >> FileName;
		if (FileName == "exit")
			break;
		else
		{
			try
			{
				Parser P(FileName.c_str());
				P.Parse();
				P.Print(FileName.substr(0, FileName.length() - 3) + "out");
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				continue;
			}
		}
	}
	return;
}

void ParserTestLaunch()
{
	char* sourceTest = "sourceTest.txt";
	ifstream FS(sourceTest);
	string FN;
	while (true)
	{
		if (FS.eof())
			break;
		getline(FS, FN);
		try
		{
			Parser P(FN.c_str());
			P.Parse();
			P.Print(FN.substr(0, FN.length() - 3) + "out");
		}
		catch (exception a)
		{
			printf_s("\n%s\n\n", a.what());
			int i = 0;
			string what(a.what());
			while (true)
			{
				if (what[i] == '"')
				{
					++i;
					while (what[i] != '"')
						++i;
					i += 2;
					break;
				}
				++i;
			}
			what = what.substr(i);
			ofstream out(FN.substr(0, FN.length() - 3) + "out");
			out << what;
			out.close();
			continue;
		}
	}
	return;
}


int main(int argc, char* argv[])
{
	cout << "Pascal Compiler by Verkholat Aleksey 2015-2016\n\n";
	if (argc == 1)
	{
		ParserLaunch();
		return 0;
	}
	if (argc == 2)
	{
		if (string(argv[1]) == "T")
		{
			TLaunch();
			return 0;
		}
		if (string(argv[1]) == "PE")
		{
			PExprLaunch();
			return 0;
		}
		if (string(argv[1]) == "PENT")
		{
			PExprLaunch_NOT_TREE();
			return 0;
		}
		if (string(argv[1]) == "PD")
		{
			PDeclLaunch();
			return 0;
		}
		if (string(argv[1]) == "PS")
		{
			PStmtLaunch();
			return 0;
		}
		if (string(argv[1]) == "PST")
		{
			PStmtTestLaunch();
			return 0;
		}
		if (string(argv[1]) == "TT")
		{
			TTestLaunch();
			return 0;
		}
		if (string(argv[1]) == "PET")
		{
			PExprTestLaunch();
			return 0;
		}
		if (string(argv[1]) == "PDT")
		{
			PDeclTestLaunch();
			return 0;
		}
		if (string(argv[1]) == "PT")
		{
			ParserTestLaunch();
			return 0;
		}
		cout << "Argument must be\n                 T PE PD PS PST TT PET PDT PENT";
		return 0;
	}
}