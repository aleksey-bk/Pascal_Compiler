#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include "Tokenizer.h"
#include "LexemeList.h"
#include "Lexeme.h"
#include "Parser.h"


//#define StdLaunch
//#define TEST_Tokenizer
//#define StdLaunch_1
#define TEST_Parser


#ifdef TEST_Tokenizer
#define TestNum 9
int main()
{
	string FileName[TestNum + 1];
	FileName[TestNum] = "exit";
	FileName[0] = "0.txt";
	FileName[1] = "1.txt";
	FileName[2] = "2.txt";
	FileName[3] = "3.txt";
	FileName[4] = "4.txt";
	FileName[5] = "5.txt";
	FileName[6] = "6.txt";
	FileName[7] = "7.txt";
	FileName[8] = "8.txt";
	FileName[TestNum] = "exit";
#define FileName FileName[i]
	int i = -1;
	while (i <= TestNum)
	{
		++i;
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
	return 0;
}
#endif
#ifdef StdLaunch
int main()
{
	cout << "Pascal Compiler by Verkholat Aleksey 2015-2016\n\n";
	cout << "Enter file name or \"exit\" for close\n\n";
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
	return 0;
}
#endif

#ifdef StdLaunch_1
int main()
{
	cout << "Pascal Compiler by Verkholat Aleksey 2015-2016\n\n";
	cout << "Enter file name or \"exit\" for close\n\n";
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
				P.PrintTree(FileName.substr(0, FileName.length() - 3) + "out");
				//printf("%lf\n", P.CalcTree());
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				continue;
			}
		}
	}
	return 0;
}
#endif
#ifdef TEST_Parser
#define TestNum 11
int main()
{
	string FileName[TestNum];
	//FileName[TestNum] = "exit";
	FileName[0] = "0.txt";
	FileName[1] = "1.txt";
	FileName[2] = "2.txt";
	FileName[3] = "err_0.txt";
	FileName[4] = "err_1.txt";
	FileName[5] = "err_2.txt";
	FileName[6] = "err_3.txt";
	FileName[7] = "err_4.txt";
	FileName[8] = "err_5.txt";
	FileName[9] = "err_6.txt";
	FileName[10] = "err_7.txt";
	cout << "Pascal Compiler by Verkholat Aleksey 2015-2016\n\n";
	cout << "Enter file name or \"exit\" for close\n\n";
	//FileName[TestNum] = "exit";
#define FileName FileName[i]
	int i = 0;
	while (i < TestNum)
	{	
		if (FileName == "exit")
			break;
		else
		{
			try
			{
				Parser P(FileName.c_str());
				P.Parse();
				P.PrintTree(FileName.substr(0, FileName.length() - 3) + "out");
				//printf("%lf\n", P.CalcTree());
			}
			catch (exception a)
			{
				printf_s("\n%s\n\n", a.what());
				++i;
				continue;
			}
		}
		++i;
	}
	return 0;
}
#endif