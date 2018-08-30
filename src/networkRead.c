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


/* Reads box information from file */
PetscErrorCode readBoxLine(FILE *file_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	char 		bChar;
	PetscInt 	nodeCount, fibreCount;
  	PetscScalar xDim, yDim, zDim; 
  	PetscInt 	xPer, yPer, zPer;

	// read in a box line
	//sscanf(file_ptr, "%c %d %d %lf %lf %lf %d %d %d",
  	//		&bChar, &nodeCount, &fibreCount, 
  	//		&xDim, &yDim, &zDim, &xPer, &yPer, &zPer);

	//box_ptr = makeBox(nodeCount, fibreCount, 
  	//					xDim, yDim, zDim, xPer, yPer, zPer);

  	networkBuild();

	return ierr;
}