#include "systemSolveTests.h"

int test_systemSolve()
{
	check(systemSolve() == 0, "systemSolve failed");
	return 0;

error:
	return -1;
}