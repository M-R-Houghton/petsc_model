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


/* Destroys the data structure */
void destroySparse(Sparse *sparse_ptr) 
{
	free(sparse_ptr->counter);
	sparse_ptr->counter = NULL; assert(sparse_ptr->counter == NULL);
	free(sparse_ptr->rowp);
	sparse_ptr->rowp = NULL; assert(sparse_ptr->rowp == NULL);
	free(sparse_ptr->col);
	sparse_ptr->col = NULL; assert(sparse_ptr->col == NULL);
	free(sparse_ptr->mat);
	sparse_ptr->mat = NULL; assert(sparse_ptr->mat == NULL);

	free(sparse_ptr);
	sparse_ptr = NULL; assert(sparse_ptr == NULL); 	/* Null checks */
}
