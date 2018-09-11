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
	PetscErrorCode 	ierr = 0;
	PetscInt 		nodeCount, fibreCount;
  	PetscScalar 	xDim, yDim, zDim; 
  	PetscInt 		xPer, yPer, zPer;

	/* read in a box line */
	sscanf(line_ptr, "%d %d %lf %lf %lf %d %d %d",
  			&nodeCount, &fibreCount, 
  			&xDim, &yDim, &zDim, &xPer, &yPer, &zPer);

	/* assign box values to scanned values */
	*box_ptr = makeBox(nodeCount, fibreCount, xDim, yDim, zDim, xPer, yPer, zPer);

	return ierr;
}


/* Reads fibre information from a given line pointer */
PetscErrorCode readFibreLine(char *line_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Reads node information from a given line pointer */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscInt *gIndex_ptr, PetscScalar gamma)
{
	PetscErrorCode 	ierr;
  	PetscInt 		nID, nType;
	PetscScalar 	x, y, z;

	/* read in a box line */
	sscanf(line_ptr, "%d %d %lf %lf %lf", &nID, &nType, &x, &y, &z);

	/* assign scanned values to a node */
	ierr = makeNode(box_ptr, nID, nType, x, y, z, gIndex_ptr, gamma);CHKERRQ(ierr);

	return ierr;
}


/* Removes trailing whitespace on the right-hand side of a string */
char *trimRightWhitespace(char *str_ptr)
{
	char *end = str_ptr + strlen(str_ptr);

  	while( (end!=str_ptr) && isspace(*(end-1)) )
  	{
  		end--;
  	}
  	*end = '\0';

  	return str_ptr;
}
