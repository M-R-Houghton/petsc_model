#include "networkBuild.h"

/* Initiates network build routine */
PetscErrorCode networkBuild()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Building network objects...\n");CHKERRQ(ierr);

	return ierr;
}