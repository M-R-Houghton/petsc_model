#include "networkWriteTests.h"

int test_networkWrite()
{
	check(networkWrite() == 0, "networkWrite failed");
	return 0;

error:
	return -1;
}