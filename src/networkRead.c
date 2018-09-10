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
PetscErrorCode readBoxLine(char *line_ptr, Box **box_ptr)
{
	PetscErrorCode ierr = 0;

	char 		bChar;
	PetscInt 	nodeCount, fibreCount;
  	PetscScalar xDim, yDim, zDim; 
  	PetscInt 	xPer, yPer, zPer;

	// read in a box line
	sscanf(line_ptr, "%c %d %d %lf %lf %lf %d %d %d",
  			&bChar, &nodeCount, &fibreCount, 
  			&xDim, &yDim, &zDim, &xPer, &yPer, &zPer);

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %c\n", bChar);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %d\n", nodeCount);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %d\n", fibreCount);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %lf\n", xDim);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %lf\n", yDim);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %lf\n", zDim);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %d\n", xPer);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %d\n", yPer);CHKERRQ(ierr);
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Reading %d\n", zPer);CHKERRQ(ierr);

	*box_ptr = makeBox(nodeCount, fibreCount, 
  						xDim, yDim, zDim, xPer, yPer, zPer);

  	//networkBuild();

	return ierr;
}