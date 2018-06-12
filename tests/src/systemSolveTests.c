#include "systemSolveTests.h"

PetscErrorCode test_systemSolve()
{
	PetscErrorCode ierr;
	ierr = systemSolve();CHKERRQ(ierr);

	return ierr;
}