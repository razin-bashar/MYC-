#include "stdafx.h"
#include "Test.h"


Test::Test()
{
}

void Test::doCall(void(*ev)(int),void(*function)(int)) {
	(*function)(10);
}

Test::~Test()
{
}
