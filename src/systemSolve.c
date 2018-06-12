#include "systemSolve.h"

/* Initiates system solve routine */
PetscErrorCode systemSolve()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Solving system...\n");CHKERRQ(ierr);

	return ierr;
}