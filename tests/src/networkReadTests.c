#include "networkReadTests.h"

const char *test_dataRead() 
{
    int foo = 7;
    mu_assert(foo == 7, "error, foo != 7");
    return 0;
}

const char *all_data() 
{
    mu_run_test(test_dataRead);
    return 0;
}

int test_networkRead()
{
	check(networkRead() == 0, "networkRead failed");

	assert(test_dataRead() == 0);
	return 0;

error:
	return -1;
}