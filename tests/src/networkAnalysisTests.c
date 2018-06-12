#include "networkAnalysisTests.h"

/* Tests network analysis routine */
PetscErrorCode test_networkAnalysis()
{
	PetscErrorCode ierr;
	ierr = networkAnalysis();CHKERRQ(ierr);
	
	return ierr;
}