#pragma once
#include "Symbol.h"
#include "SymType.h"
#include "SymVar.h"
#include "SymTypeDinArray.h"
#include "SymTypeArray.h"
#include "ExprBinOp.h"
#include"ExprIntConst.h"
#include "ExprRealConst.h"
#include "SymTypeAlias.h"
#include "SymTypeInteger.h"
#include "SymTypeFloat.h"
#include "SymTypeChar.h"
#include "SymTypeRecord.h"
#include "SymProc.h"
#include <fstream>
#include <vector>


class Symtable
{
private:
	vector<SymType*> table_types;
	vector<SymVar*> table_vars;
public:
	SymType* SearchType(string name);
	SymVar* SearchVar(string name);
	SymProc* SearchProc(string name);
	void AddSymVar(SymVar* a);
	void AddSymType(SymType* a);
	string RecToCode(SymType* t);
	string* GetPrintVars();
	string* GetPrintTypes();
	string* GetPrintConst();
	void PrintVal(string* ws, ValStruct* v, bool arr_el);
	string TypeToCode(SymType* t);
	Symtable();
	~Symtable();
};

