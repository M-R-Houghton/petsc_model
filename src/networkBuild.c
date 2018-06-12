#include "networkBuild.h"

/* Initiates network build routine */
PetscErrorCode networkBuild()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Building network objects...\n");CHKERRQ(ierr);

	return ierr;
}


/* function for parameters creation */
Parameters *makeParameters(PetscScalar gamma, PetscScalar yMod)
{
	/* allocate memory */
	Parameters *par_ptr = (Parameters *)malloc(sizeof(Parameters));

	/* assign attributes */
	par_ptr->gamma = gamma;
	par_ptr->youngsModulus = yMod;

	return par_ptr;
}