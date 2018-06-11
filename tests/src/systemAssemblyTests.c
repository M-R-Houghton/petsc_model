#include "systemAssemblyTests.h"

int test_systemAssembly()
{
	PetscErrorCode 	ierr;
	Mat 			dummyMat = NULL;
	PetscInt		n = 10;
	ierr = MatCreate(PETSC_COMM_WORLD,&dummyMat);CHKERRQ(ierr);
	ierr = MatSetSizes(dummyMat,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr = MatSetFromOptions(dummyMat);CHKERRQ(ierr);
	ierr = MatSetUp(dummyMat);CHKERRQ(ierr);
	assert(systemAssembly(dummyMat) == 0);
	ierr = MatDestroy(&dummyMat);CHKERRQ(ierr);
	return 0;
}