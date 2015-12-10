#pragma once
#include <string>
using namespace std;

enum { sym_type, sym_proc, sym_var };

class Symbol
{
public:
	string name;
	int class_type;
	Symbol()
	{
		name = "";
	}
	~Symbol(){};
};

