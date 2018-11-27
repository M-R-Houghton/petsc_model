#include "networkRead.h"

/* Initiates network read in routine */
PetscErrorCode networkRead(const char *fileToRead_ptr, Box **box_ptr_ptr, PetscScalar gamma)
{
	PetscErrorCode 	ierr = 0;
    PetscBool       coupledSystem = PETSC_FALSE;

	/* declare array for storing line, pointer, and counter for current line */
	char line[MAX_LENGTH], *line_ptr;
	PetscInt line_number = 0;
	FILE *fp;

	/* setup global index */
	PetscInt gIndex = 0;
	PetscInt *gIndex_ptr = &gIndex;

	/* open file and check whether successful */
	fp = fopen(fileToRead_ptr, "r");
	if (fp == NULL) SETERRQ(PETSC_COMM_WORLD,65,"Error in opening file.");

	/* read in line by line until EOF is reached */
	while ((line_ptr = fgets(line, sizeof(line), fp)) != NULL)
	{
		/* read in line and increment line number */
		ierr = readDataLine(line_ptr, box_ptr_ptr, gIndex_ptr, gamma);CHKERRQ(ierr);
		line_number += 1;
	}

    /* produce numbering for internal nodes */
    ierr = setInternalNodeIndices(*box_ptr_ptr, coupledSystem);CHKERRQ(ierr);

	/* use final global index to set total internal nodes */
	(*box_ptr_ptr)->nodeInternalCount = gIndex;

	/* close file */
	fclose(fp);

	return ierr;
}


PetscErrorCode setInternalNodeIndices(Box *box_ptr, PetscBool coupledSystem)
{
    PetscErrorCode ierr = 0;

    if (coupledSystem)
    {
        /* coupled numbering */
    }
    else 
    {
        /* standard numbering */
        ierr = setStandardInternalNodeIndices(box_ptr);CHKERRQ(ierr);
    }

    return ierr;
}


PetscErrorCode setStandardInternalNodeIndices(Box *box_ptr)
{
    PetscErrorCode  ierr = 0;
    PetscInt        i, newIndex=0;
    /* loop over every node of the network */
    for (i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node_ptr = &(box_ptr->masterNodeList[i]);
        if (node_ptr->globalID == -2)
        {
            assert(node_ptr->nodeType == NODE_INTERNAL);
            node_ptr->globalID = newIndex;
            newIndex += 1;
        }
    }        

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


/* Reads node coupling information from a given line pointer */
PetscErrorCode readCouplingLine(char *line_ptr, Box *box_ptr)
{
	PetscErrorCode 	ierr = 0;
  	PetscInt 		nID1, nID2;

	/* read in a node coupling line */
	sscanf(line_ptr, "%d %d", &nID1, &nID2);

	/* assign scanned values to a node */
//	ierr = makeNode(box_ptr, nID, nType, x, y, z, gIndex_ptr, gamma);CHKERRQ(ierr);

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
