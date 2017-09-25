#pragma once
#include <string>
class Collections
{
public:
	Collections();
	static char* strtochar(std::string str);
	static std::string chartostr(char* chr);
	static char* chrconcat(char* a, char* b);
	~Collections();
};

