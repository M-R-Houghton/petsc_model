#include "networkWrite.h"

PetscErrorCode networkWrite()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Writing to file...\n");CHKERRQ(ierr);

	return ierr;
}