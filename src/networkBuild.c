#include "networkBuild.h"

PetscErrorCode networkBuild()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Building network objects...\n");CHKERRQ(ierr);

	return ierr;
}