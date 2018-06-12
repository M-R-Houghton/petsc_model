#include "networkWriteTests.h"

/* Tests network write out routine */
PetscErrorCode test_networkWrite()
{
	PetscErrorCode ierr;
	ierr = networkWrite();CHKERRQ(ierr);
	
	return ierr;
}