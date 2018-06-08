#include "systemAssemblyTests.h"

int test_systemAssembly()
{
	PetscErrorCode 	ierr;
	Mat 			dummyMat = NULL;
	assert(systemAssembly(dummyMat) == 0);
	ierr = MatDestroy(&dummyMat);CHKERRQ(ierr);
	return 0;
}