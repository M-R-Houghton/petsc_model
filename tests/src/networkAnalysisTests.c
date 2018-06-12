#include "networkAnalysisTests.h"

PetscErrorCode test_networkAnalysis()
{
	PetscErrorCode ierr;
	ierr = networkAnalysis();CHKERRQ(ierr);
	
	return ierr;
}