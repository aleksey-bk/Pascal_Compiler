#pragma once
#include "Tokenizer.h"
#include "Expression.h"
#include "ExprBinOp.h"
#include "ExprIntConst.h"
#include "ExprRealConst.h"
#include "ExprVar.h"
#include "ExprCharConst.h"
#include "ExprArgs.h"
#include "ExprUnOp.h"
#include "SymTypeArray.h"
#include "SymTypeDinArray.h"
#include "Symtable.h"
#include "SymTypeAlias.h"
#include "SymTypeScalar.h"
#include "SymTypeInteger.h"
#include "SymTypeFloat.h"
#include "SymTypeChar.h"
#include "SymTypeRecord.h"
#include "SymVarConst.h"
#include "SymVarGlobal.h"
#include "SymTypePointer.h"
#include "StmtAssign.h"
#include "StmtBlock.h"
#include "StmtCall.h"
#include "StmtFor.h"
#include "StmtIf.h"
#include "StmtWhile.h"
#include "StmtContinue.h"
#include "StmtBreak.h"
#include <vector>
#include <queue>
#include <stack>

#define MaxPriority 4
#define MaxNumOperatorsInP 7
enum priority_operations { assignP, cmp_inP, plus_orP, mul_mod_andP, func_rec_arrP };

class Parser
{	
	friend class Expression;
private:
	Symtable Table;
	Tokenizer T;
	queue<Lexeme> LexBuffer;
	Lexeme LastLex;
	Lexeme SaveLex;
	Expression* root;
	Expression* ParseExpr(int priority, Expression* L);
	Expression* ParseFactor();
	Expression* ParseAssign(Expression* l);
	Expression* ParseRecord(Expression* l);
	Expression* ParseFunction(Expression* l);
	Expression* ParseArray(Expression* l);
	Expression* Parsein(Expression* left);
	Expression* ParseCast(Expression* l);
	Expression* ParseBinOp(Expression*l, int op_idx, int priority);
	Expression* ParseIn(Expression* l);  
	Expression* ParseDpz();
	SymType* FindType(Expression* l);
	SymType* FindVarType(Expression* v);
	bool CheckTypeEq(SymType* l, SymType* r);
	bool CheckTypeComp(SymType* l, SymType* r);
	exception GetErrInformation(Lexeme l, string inf);
	void CheckSign(Lexeme S);
	void CheckOperand(Lexeme S);
	bool dot_com_flag;
	bool IsTypename(string s);
	int IsUnaryOp(Lexeme l);
	bool ParseArgsStatus;
	void ParseDeclaration();
	bool IsSectionName(Lexeme L);
	void ParseVarSection();
	void ParseConstSection();
	void ParseTypeSection();
	void ParseRecord(string rec_name);
	void ParseSection(string section_name);
	SymType* SetExprAriphmType(Expression* l, Expression* r, int op);
	void SetValToType(SymType* t, Expression* v);
	SymType* ParseType();
	ValStruct* ParseInit(SymType* t);
	SymType* MAllocation(SymType* t);
	Lexeme NextLex();
	Statement* stmt_root;
	void CheckSemicolon(bool check_semicolon_before_else = false);
	Statement* ParseStmt(bool check_semicolon_before_else = false, bool main_block = false);
	Statement* ParseIf();
	Statement* ParseWhile();
	Statement* ParseFor();
	Statement* ParseBlock(bool check_semicolon_before_else = false);
	Statement* ParseCall();
	Statement* ParseAssign();
	bool IsLoopVariable(string var_name);
	list<string> LoopVariables;
	int LoopCounter;
	void PrintNode(Expression* node, int h, string* ws);
	void PrintStmtNode(Statement* s, string* ws, int spaces);
	void PrintExpr(string FileName, Expression* expr, bool clean_file = false);
	string* PrintExprPart(Expression* expr);
public:
	static const int priority_length[MaxPriority + 1];
	static const int priority_op[MaxPriority + 1][MaxNumOperatorsInP];
	static double Calc(Expression* e);
	static int CalcLength(Expression* e);
	void Parse();
	void ParserExprTEST();
	void ParserDeclTEST();
	void ParserStmtTEST();
	void Print(string FileName);
	void PrintExprMain(string FileName);
	void PrintExprTree(string FileName, bool clean_file = false);
	void PrintDecl(string FileName, bool clean_file = false);
	void PrintStmt(string FileName, bool clean_file = false);
	Parser(const char* FileName);
	~Parser();
};



