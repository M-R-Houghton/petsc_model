#include "systemAssemblyTests.h"

PetscErrorCode test_systemAssembly()
{
	PetscErrorCode 	ierr;
	Mat 			dummyMat = NULL;
	PetscInt		n = 10;
	ierr = MatCreate(PETSC_COMM_WORLD,&dummyMat);CHKERRQ(ierr);
	ierr = MatSetSizes(dummyMat,PETSC_DECIDE,PETSC_DECIDE,n,n);CHKERRQ(ierr);
	ierr = MatSetFromOptions(dummyMat);CHKERRQ(ierr);
	ierr = MatSetUp(dummyMat);CHKERRQ(ierr);
	ierr = systemAssembly(dummyMat);CHKERRQ(ierr);
	ierr = MatDestroy(&dummyMat);CHKERRQ(ierr);
	return ierr;
}