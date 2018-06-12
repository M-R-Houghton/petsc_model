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
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}

/* Tests sparse matrix destroyer */
PetscErrorCode test_destroySparse()
{
	PetscErrorCode ierr;
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}

/* Tests box destroyer */
PetscErrorCode test_destroyBox()
{
	PetscErrorCode ierr;
	ierr = 0;CHKERRQ(ierr);
	
	return ierr;
}
