#include "networkDestroy.h"

/* Initiates network destroy routine */
PetscErrorCode networkDestroy()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Destroying network objects...\n");CHKERRQ(ierr);

	return ierr;
}