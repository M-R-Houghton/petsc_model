#include "systemAssemblyTests.h"

int test_systemAssembly()
{
	check(systemAssembly() == 0, "systemAssembly failed");
	return 0;

error:
	return -1;
}