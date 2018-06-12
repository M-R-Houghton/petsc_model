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

PetscErrorCode test_networkRead()
{
	PetscErrorCode ierr;
	ierr = networkRead();CHKERRQ(ierr);

	assert(test_dataRead() == 0);
	return ierr;
}