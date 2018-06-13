#include "networkDestroyTests.h"

/* Tests network destroy routine */
PetscErrorCode test_networkDestroy()
{
	PetscErrorCode ierr;

	ierr = networkDestroy();CHKERRQ(ierr);

	ierr = test_destroyParameters();CHKERRQ(ierr);
	ierr = test_destroySparse();CHKERRQ(ierr);
	ierr = test_destroyBox();CHKERRQ(ierr);
	
	return ierr;
}

/* Tests parameter destroyer */
PetscErrorCode test_destroyParameters()
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] destroyParameters...\n");CHKERRQ(ierr);

	Parameters *par_ptr 	= (Parameters *)malloc(sizeof(Parameters));
	par_ptr->gamma 			= 0.5;
	par_ptr->youngsModulus 	= 1.0;

	destroyParameters(par_ptr);
	
	return ierr;
}

/* Tests sparse matrix destroyer */
PetscErrorCode test_destroySparse()
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] destroySparse...\n");CHKERRQ(ierr);

	Sparse *sparse_ptr  = (Sparse *)malloc(sizeof(Sparse));
	sparse_ptr->n 		= 25;
	sparse_ptr->nz 		= 15;
	sparse_ptr->counter = (PetscInt *)calloc(sparse_ptr->n,sizeof(PetscInt));
	sparse_ptr->rowp    = (PetscInt *)calloc(sparse_ptr->n+1,sizeof(PetscInt));
	sparse_ptr->col 	= (PetscInt *)calloc(sparse_ptr->nz,sizeof(PetscInt));
	sparse_ptr->mat 	= (PetscScalar *)calloc(sparse_ptr->nz,sizeof(PetscScalar));

	destroySparse(sparse_ptr);
	
	return ierr;
}

/* Tests box destroyer */
PetscErrorCode test_destroyBox()
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] destroyBox...\n");CHKERRQ(ierr);
	
	return ierr;
}
