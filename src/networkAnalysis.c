#include "networkAnalysis.h"

PetscErrorCode networkAnalysis()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Analysing network...\n");CHKERRQ(ierr);
	
	return ierr;
}