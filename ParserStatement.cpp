#include "Parser.h"

Statement* Parser::ParseIf()
{
	Expression* if_expr = ParseExpr(cmp_inP, NULL);
	if (SaveLex.lexid != KWthen)
		throw GetErrInformation(SaveLex, "expected \"then\"");
	SaveLex.lexid = -1;
	if (if_expr->expr_type != Table.SearchType("integer"))
		throw GetErrInformation(LastLex, "expression type must be integer");
	StmtIf* stmt = new StmtIf;
	stmt->if_expr = if_expr;
	stmt->if_block = ParseStmt(true);
	NextLex();
	if (LastLex.lexid == KWelse)
		stmt->else_block = ParseStmt();
	else
		LexBuffer.push(LastLex);
	return stmt;
}



Statement* Parser::ParseWhile()
{
	Expression* while_expr = ParseExpr(cmp_inP, NULL);
	if (SaveLex.lexid != KWdo)
		throw GetErrInformation(SaveLex, "expected \"then\"");
	SaveLex.lexid = -1;
	++LoopCounter;
	StmtWhile* stmt = new StmtWhile;
	stmt->while_expr = while_expr;
	stmt->while_block = ParseStmt();
	--LoopCounter;
	return stmt;
}

Statement* Parser::ParseFor()
{
	NextLex();
	if (Table.SearchVar(LastLex.text) == NULL)
		throw GetErrInformation(LastLex, "unknowm identifier");
	LoopVariables.push_front(LastLex.text);
	++LoopCounter;
	ExprVar* var = new ExprVar(LastLex.text, Table.SearchVar(LastLex.text)->var_type);
	if (var->expr_type != Table.SearchType("integer"))
		throw GetErrInformation(LastLex, "var type must be integer");
	NextLex();
	if (LastLex.lexid != assign)
		throw GetErrInformation(LastLex, "expected \":=\"");
	Expression* e1 = ParseExpr(cmp_inP, NULL);
	if (e1->expr_type != var->expr_type)
		throw GetErrInformation(LastLex, "expression type must be integer");
	if (SaveLex.lexid != KWto && SaveLex.lexid != KWdownto)
		throw GetErrInformation(SaveLex, "expected keyword \"to\" or \"downto\"");
	bool to = true;
	if (SaveLex.lexid == KWdownto)
		to = false;
	SaveLex.lexid = -1;
	Expression* e2 = ParseExpr(cmp_inP, NULL);
	if (e2->expr_type != var->expr_type)
		throw GetErrInformation(LastLex, "expression type must be integer");
	if (SaveLex.lexid != KWdo)
		throw GetErrInformation(SaveLex, "expected keyword \"do\"");
	SaveLex.lexid = -1;
	StmtFor* f = new StmtFor(to);
	f->expr_1 = e1;
	f->expr_2 = e2;
	f->v = var;
	f->for_block = ParseStmt();
	LoopVariables.pop_front();
	--LoopCounter;
	return f;
}


Statement* Parser::ParseAssign()
{
	Lexeme prev = LastLex;
	NextLex();
	if (LastLex.lexid != assign && LastLex.lexid != open_qbr && LastLex.lexid != rec)
		throw GetErrInformation(LastLex, "expected \":=\"");
	if (LastLex.lexid == assign)
	{
		if (IsLoopVariable(prev.text))
			throw GetErrInformation(prev, "assign to loop variable");
	}
	LexBuffer.push(prev);
	LexBuffer.push(LastLex);
	Expression* e = ParseExpr(assignP, NULL);
	LexBuffer.push(SaveLex);
	SaveLex.lexid = -1;
	StmtAssign* stmt = new StmtAssign;
	stmt->assign_expr = e;
	return stmt;
}

Statement* Parser::ParseCall()
{
	LexBuffer.push(LastLex);
	Expression* proc = ParseExpr(func_rec_arrP, NULL);
	LexBuffer.push(SaveLex);
	StmtCall* stmt = new StmtCall;
	stmt->proc_expr = (ExprBinOp*)proc;
	return stmt;
}

Statement* Parser::ParseBlock(bool check_semicolon_before_else)
{
	StmtBlock* b = new StmtBlock();
	while (true)
	{
		NextLex();
		if (LastLex.lexid != KWend)
			LexBuffer.push(LastLex);
		else
			return b;
		b->SList.push_back(ParseStmt(check_semicolon_before_else));
	}
}

void Parser::CheckSemicolon(bool check_semicolon_before_else)
{
	NextLex();
	if (!check_semicolon_before_else)
	{
		if (LastLex.lexid != dot_com)
			throw GetErrInformation(LastLex, "expected \";\"");
	}
	else
	{
		if (LastLex.lexid == dot_com)
		{
			NextLex();
			if (LastLex.lexid == KWelse)
				throw GetErrInformation(LastLex, "bad \";\"");
			LexBuffer.push(LastLex);
			return;
		}
		if (LastLex.lexid == KWelse)
		{
			LexBuffer.push(LastLex);
			return;
		}
		throw GetErrInformation(LastLex, "expected \"else\" or \";\"");
	}
	return;
}

void Parser::ParserStmtTEST()
{
	stmt_root = ParseStmt();
	NextLex();
	if (LastLex.type != TypeEOF)
		throw GetErrInformation(LastLex, "bad lex");
	return;
}


Statement* Parser::ParseStmt(bool check_semicolon_before_else, bool main_block)
{
	Statement* ret;
	NextLex();
	if (LastLex.type == TypeIdent)
	{
		if (Table.SearchProc(LastLex.text) != NULL)
		{
			ret = ParseCall();
			CheckSemicolon(check_semicolon_before_else);
			return ret;
		}
		if (Table.SearchVar(LastLex.text) != NULL)
		{
			ret = ParseAssign();
			CheckSemicolon(check_semicolon_before_else);
			return ret;
		}
		throw GetErrInformation(LastLex, "unknown identifier");
	}
	if (LastLex.type == TypeKeyword)
	{
		switch (LastLex.lexid)
		{
		case KWif:
		{
			ret = ParseIf();
			return ret;
		}
		case KWfor:
		{
			ret = ParseFor();
			return ret;
		}
		case KWwhile:
		{
			ret = ParseWhile();
			return ret;
		}
		case KWbegin:
		{
			ret = ParseBlock();
			NextLex();
			if (LastLex.type == TypeDotEnd)
			{
				if (!main_block)
					throw GetErrInformation(LastLex, "expected \";\"");
				return ret;
			}
			else
				LexBuffer.push(LastLex);
			CheckSemicolon(check_semicolon_before_else);
			return ret;
		}
		case KWcontinue:
		{
			if (LoopCounter == 0)
				throw GetErrInformation(LastLex, "no loop");
			CheckSemicolon(check_semicolon_before_else);
			return new StmtContinue;
		}
		case KWbreak:
		{
			if (LoopCounter == 0)
				throw GetErrInformation(LastLex, "no loop");
			CheckSemicolon(check_semicolon_before_else);
			return new StmtBreak;
		}
		default:
		{
			throw GetErrInformation(LastLex, "bad lex");
		}
		}
	}
	throw GetErrInformation(LastLex, "unexpected lex");
}

bool Parser::IsLoopVariable(string var_name)
{
	if (LoopVariables.size() == 0)
		return false;
	list<string>::iterator a = LoopVariables.begin();
	list<string>::iterator b = LoopVariables.end();
	while (true)
	{
		if (*a == var_name)
			return true;
		a++;
		if (a == b)
			return false;
	}
}

void Parser::PrintStmtNode(Statement* s, string* ws, int spaces)  //_______________________________________________________
{
	for (int i = 0; i < spaces; ++i)
		*ws += " ";
	switch (s->class_id)
	{
	case Statement::CLASS_ASSIGN:
	{
		StmtAssign* a = (StmtAssign*)s;
		string* s = PrintExprPart(a->assign_expr);
		*ws += *s + ";\n";
		delete s;
		break;
	}
	case Statement::CLASS_BLOCK:
	{
		StmtBlock* b = (StmtBlock*) s;
		*ws += "begin\n";
		list<Statement*>::iterator it = b->SList.begin();
		list<Statement*>::iterator end = b->SList.end();
		if (b->SList.empty())
		{
			for (int i = 0; i < spaces; ++i)
				*ws += " ";
			if (b != (StmtBlock*)stmt_root)
				*ws += "end;\n";
			else
				*ws += "end.";
			break;
		}
		while (true)
		{
			PrintStmtNode(*it, ws, spaces + 2);
			++it;
			if (it == end)
				break;
		}
		for (int i = 0; i < spaces; ++i)
			*ws += " ";
		if (b != (StmtBlock*)stmt_root)
			*ws += "end;\n";
		else
			*ws += "end.";
		break;
	}
	case Statement::CLASS_BREAK:
	{
		*ws += "break;\n";
		break;
	}
	case Statement::CLASS_CALL:
	{
		StmtCall* c = (StmtCall*)s;
		*ws += ((ExprVar*)((ExprBinOp*)(c->proc_expr))->left)->name;
		string* s = PrintExprPart(c->proc_expr->right);
		*ws += *s + ";";
		delete s;
		break;
	}
	case Statement::CLASS_CONTINUE:
	{
		*ws += "continue;\n";
		break;
	}
	case Statement::CLASS_FOR:
	{
		StmtFor* f = (StmtFor*)s;
		*ws += "for ";
		*ws += f->v->name + " := ";
		string* s = PrintExprPart(f->expr_1);
		*ws += *s + " ";
		delete s;
		if (f->to)
			*ws += "to ";
		else
			*ws += "downto ";
		s = PrintExprPart(f->expr_2);
		*ws += *s + " do\n";
		delete s;
		PrintStmtNode(f->for_block, ws, spaces + 2);
		break;
	}
	case Statement::CLASS_IF:
	{
		StmtIf* i = (StmtIf*)s;
		string* s = PrintExprPart(i->if_expr);
		*ws += "if " + *s + " then\n";
		delete s;
		PrintStmtNode(i->if_block, ws, spaces + 2);
		if (i->else_block != NULL)
		{
			*ws = ws->substr(0, ws->length() - 2) + "\n";
			for (int i = 0; i < spaces; ++i)
				*ws += " ";
			*ws += "else\n";
			PrintStmtNode(i->else_block, ws, spaces + 2);
		}
		break;
	}
	case Statement::CLASS_WHILE:
	{
		StmtWhile* w = (StmtWhile*)s;
		string* s = PrintExprPart(w->while_expr);
		*ws += "while " + *s + " do\n";
		delete s;
		PrintStmtNode(w->while_block, ws, spaces + 2);
		break;
	}
	}
	return;
}

void Parser::PrintStmt(string FileName, bool clean_file)
{
	if (clean_file)
	{
		ofstream s(FileName, ios_base::trunc);
		s.close();
	}
	ofstream out_stream(FileName, ios_base::app);
	string* ws = new string;
	PrintStmtNode(stmt_root, ws, 0);
	out_stream << ws->c_str();
	delete ws;
	out_stream.close();
	return;
}
