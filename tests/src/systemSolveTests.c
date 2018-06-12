#include "systemSolveTests.h"

/* Tests system solve routine */
PetscErrorCode test_systemSolve()
{
	PetscErrorCode ierr;
	ierr = systemSolve();CHKERRQ(ierr);

	return ierr;
}