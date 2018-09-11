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


/* Reads box information from a given line pointer */
PetscErrorCode readBoxLine(char *line_ptr, Box **box_ptr)
{
	PetscErrorCode ierr = 0;

	char 		lineChar;
	PetscInt 	nodeCount, fibreCount;
  	PetscScalar xDim, yDim, zDim; 
  	PetscInt 	xPer, yPer, zPer;

	/* read in a box line */
	sscanf(line_ptr, "%c %d %d %lf %lf %lf %d %d %d",
  			&lineChar, &nodeCount, &fibreCount, 
  			&xDim, &yDim, &zDim, &xPer, &yPer, &zPer);

	/* assert that it is a box line */
	//int myInt = atoi(&lineChar);			/* does not behave as I would expect */
	//assert(myInt == 98);					/* return to this at some point and understand why */

	/* assign box values to scanned values */
	*box_ptr = makeBox(nodeCount, fibreCount, xDim, yDim, zDim, xPer, yPer, zPer);

	return ierr;
}


/* Reads node information from a given line pointer */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscInt *gIndex_ptr, PetscScalar gamma)
{
	PetscErrorCode ierr;

	char 		lineChar;
  	PetscInt 	nID, nType;
	PetscScalar x, y, z;

	/* read in a box line */
	sscanf(line_ptr, "%c %d %d %lf %lf %lf",
  			&lineChar, &nID, &nType, &x, &y, &z);

	/* assert that it is a node line */
	//assert(lineChar == 'n');

	/* assign scanned values to a node */
	ierr = makeNode(box_ptr, nID, nType, x, y, z, gIndex_ptr, gamma);CHKERRQ(ierr);

	return ierr;
}