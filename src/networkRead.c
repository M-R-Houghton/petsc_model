#include "networkRead.h"

/* Initiates network read in routine */
PetscErrorCode networkRead()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading from file...\n");CHKERRQ(ierr);

	/* open file */

	/* read in line by line */

		/* process box line */

		/* process fibre line */
		/* make fibre */

		/* process node line */
		/* make node */

	/* close file  */

	return ierr;
}