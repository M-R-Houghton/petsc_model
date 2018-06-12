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
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}

/* Tests sparse matrix builder */
PetscErrorCode test_makeSparse()
{
	PetscErrorCode ierr;
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}

/* Tests box builder */
PetscErrorCode test_makeBox()
{
	PetscErrorCode ierr;
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}

/* Tests fibre builder */
PetscErrorCode test_makeFibre()
{
	PetscErrorCode ierr;
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}

/* Tests node builder */
PetscErrorCode test_makeNode()
{
	PetscErrorCode ierr;
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}