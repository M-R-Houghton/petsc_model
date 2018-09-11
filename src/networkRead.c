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


/* Reads network data from a given line pointer */
PetscErrorCode readDataLine(char *line_ptr, Box **box_ptr_ptr, PetscInt *gIndex_ptr, PetscScalar gamma)
{
	PetscErrorCode ierr = 0;

	/* collect initial character and move pointer to where cropped line begins */
    char *tkn_ptr        = strtok(line_ptr, " ");
    char *lineCrop_ptr   = tkn_ptr + 2;
	
	/* switch over different line types */
	switch (*tkn_ptr)
	{
		case 'b':
			/* pass line pointer to box line reader */
			readBoxLine(lineCrop_ptr, box_ptr_ptr);
			break;
		case 'f':
			/* pass line pointer to fibre line reader */
			readFibreLine(lineCrop_ptr, *box_ptr_ptr);
			break;
		case 'n':
			/* pass line pointer to node line reader */
			readNodeLine(lineCrop_ptr, *box_ptr_ptr, gIndex_ptr, gamma);
			break;
		default:
			SETERRQ(PETSC_COMM_WORLD,63,"Error in identifying line type. Line size may be insufficient.");
	}

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
	PetscErrorCode ierr;

	/* preprocess line by removing rhs trailing whitespace */
	char *trimmedLine_ptr = trimRightWhitespace(line_ptr);
	
	/* declare array to store tokens of fibre information */
	char *fibreInfoArray[strlen(trimmedLine_ptr)+1];

	/* split string once and store token */
	char *tkn_ptr = strtok(trimmedLine_ptr, " ");

	PetscInt splitCounter = 0;
	while(tkn_ptr != NULL)	/* while token exists */
	{
		/* add token to array and split string again */
		fibreInfoArray[splitCounter] = tkn_ptr;
		tkn_ptr = strtok(NULL, " ");
		splitCounter += 1;
	}

	/* determine the first few fibre attributes */
	PetscInt fibreID 		= atoi(fibreInfoArray[0]);
	PetscScalar radius 		= atof(fibreInfoArray[1]);

	/* total nodes is total splits -1 for ID and -1 for radius
	 * ...where total splits includes split at end of line     */
	PetscInt nodesOnFibre 	= splitCounter - 2;

	/* allocate storage for list of nodes on fibre */
	Node **nodeList_ptr_ptr = (Node**)calloc(nodesOnFibre, sizeof(Node*));

	PetscInt fibreInfoIndex = 2;		/* move past ID and radius */
	PetscInt nodeListIndex  = 0;		/* set counter for node list index */

	/* loop through node IDs in the fibre information array */
	for (fibreInfoIndex = 2; fibreInfoIndex < splitCounter; fibreInfoIndex++)
	{
		/* typecast to find node ID */
		PetscInt nodeID = atoi(fibreInfoArray[fibreInfoIndex]);

		/* store node pointer in node list */
		nodeList_ptr_ptr[nodeListIndex] = &(box_ptr->masterNodeList[nodeID]);

		nodeListIndex += 1;
	}

	/* create a fibre using read-in information */
	ierr = makeFibre(box_ptr, fibreID, nodesOnFibre, radius, nodeList_ptr_ptr);CHKERRQ(ierr);

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
