#include "Collections.h"



Collections::Collections()
{
}

char* Collections::strtochar(std::string str) {
	char * chr = (char *)alloca(str.size() + 1);
	memcpy(chr, str.c_str(), str.size() + 1);
	return chr;
}
std::string Collections::chartostr(char* chr) {
	std::string str = std::string(chr);
	return str;
}
char* Collections::chrconcat(char* a, char* b) {
	std::string s1 = std::string(a);
	std::string s2 = std::string(b);
	s1.append(s2);
	return strtochar(s1);
}

Collections::~Collections()
{
}
