#include "networkReadTests.h"

int test_networkRead()
{
	check(networkRead() == 0, "networkRead failed");
	return 0;

error:
	return -1;
}