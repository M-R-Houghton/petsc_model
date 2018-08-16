#include "networkBuildTests.h"

/* Tests network build routine */
PetscErrorCode test_networkBuild()
{
	PetscErrorCode ierr;
	ierr = networkBuild();CHKERRQ(ierr);

	ierr = test_makeParameters();CHKERRQ(ierr);
	ierr = test_makeSparse();CHKERRQ(ierr);
	ierr = test_checkBoxArguments();CHKERRQ(ierr);
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
	
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] makeSparse...\n");CHKERRQ(ierr);
	
	return ierr;
}


/* Tests box argument checker */
PetscErrorCode test_checkBoxArguments()
{
	PetscErrorCode ierr;
	
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[TESTING] checkBoxArguments...\n");CHKERRQ(ierr);
	
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

