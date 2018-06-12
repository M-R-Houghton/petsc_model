#include "networkDestroy.h"

/* Initiates network destroy routine */
PetscErrorCode networkDestroy()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Destroying network objects...\n");CHKERRQ(ierr);

	return ierr;
}


/* Destroys a parameters structure */
void destroyParameters(Parameters *par_ptr)
{
	free(par_ptr);
	par_ptr = NULL; assert(par_ptr == NULL); 	/* Null check */
}