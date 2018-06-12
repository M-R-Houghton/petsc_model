#include "networkWriteTests.h"

PetscErrorCode test_networkWrite()
{
	PetscErrorCode ierr;
	ierr = networkWrite();CHKERRQ(ierr);
	
	return ierr;
}