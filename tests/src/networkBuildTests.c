#include "networkBuildTests.h"

/* Tests network build routine */
PetscErrorCode test_networkBuild()
{
	PetscErrorCode ierr;
	ierr = networkBuild();CHKERRQ(ierr);

	ierr = test_makeParameters();CHKERRQ(ierr);
	ierr = test_makeSparse();CHKERRQ(ierr);
	ierr = test_makeBox();CHKERRQ(ierr);
	ierr = test_makeFibre();CHKERRQ(ierr);
	ierr = test_makeNode();CHKERRQ(ierr);
	
	return ierr;
}

/* Tests parameter builder */
PetscErrorCode test_makeParameters()
{
	PetscErrorCode ierr;
	PetscScalar    gamma = 0.5, yMod = 1.0;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] makeParameters...\n");CHKERRQ(ierr);

	Parameters *par_ptr = makeParameters(gamma,yMod);
	assert(par_ptr->gamma         == 0.5);
	assert(par_ptr->youngsModulus == 1.0);

	free(par_ptr); par_ptr = NULL;
	assert(par_ptr == NULL);
	
	return ierr;
}

/* Tests sparse matrix builder */
PetscErrorCode test_makeSparse()
{
	PetscErrorCode ierr;
	PetscInt 	   n = 25, nz = 15;
	
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] makeSparse...\n");CHKERRQ(ierr);

	assert(makeSparse(10,15) == NULL);		/* test invalid arguments */

	Sparse *sparse_ptr = makeSparse(n,nz);
	assert(sparse_ptr->n  == 25);
	assert(sparse_ptr->nz == 15);			/* test inputs */

	sparse_ptr->col[14] = 82;
	assert(sparse_ptr->col[14] == 82);		/* test assignment */

	/* clean up and do null checks */
	free(sparse_ptr->counter); sparse_ptr->counter = NULL;
	assert(sparse_ptr->counter == NULL);
	free(sparse_ptr->rowp); sparse_ptr->rowp = NULL;
	assert(sparse_ptr->rowp == NULL);
	free(sparse_ptr->col); sparse_ptr->col = NULL;
	assert(sparse_ptr->col == NULL);
	free(sparse_ptr->mat); sparse_ptr->mat = NULL;
	assert(sparse_ptr->mat == NULL);
	free(sparse_ptr); sparse_ptr = NULL;
	assert(sparse_ptr == NULL);
	
	return ierr;
}

/* Tests box builder */
PetscErrorCode test_makeBox()
{
	PetscErrorCode ierr;
	
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] makeBox...\n");CHKERRQ(ierr);
	
	return ierr;
}

/* Tests fibre builder */
PetscErrorCode test_makeFibre()
{
	PetscErrorCode ierr;
	
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] makeFibre...\n");CHKERRQ(ierr);
	
	return ierr;
}

/* Tests node builder */
PetscErrorCode test_makeNode()
{
	PetscErrorCode ierr;
	
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] makeNode...\n");CHKERRQ(ierr);
	
	return ierr;
}