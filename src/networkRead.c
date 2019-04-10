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

    readInputFile(fileToRead_ptr, box_ptr_ptr, coupledSystem, gIndex_ptr, gamma);

    /* read data line counts the number of couples in gIndex_ptr */ 
    if (*gIndex_ptr > 0) coupledSystem = PETSC_TRUE;

    /* 
     * WARNING: This is all still very messy and needs to be made much cleaner!!!
     * WARNING: Don't forget to also add in the unit tests as you go!
     * WARNING: Get rid of as much of the duplication as possible!
     */
    if (coupledSystem)
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD,"YES WE HAVE A COUPLED SYSTEM\n");CHKERRQ(ierr);

        /* sanity check: couples not counted if this fails */
        assert(gIndex != 0);
        assert(gIndex == *gIndex_ptr);

        /* use counted couples to allocate master couple array */
        (*box_ptr_ptr)->masterCoupleList = (Couple*)calloc(gIndex, sizeof(Couple));

        /* couple count now to be used as index */
        *gIndex_ptr = 0;

        readInputFile(fileToRead_ptr, box_ptr_ptr, coupledSystem, gIndex_ptr, gamma);
        
        // For debugging only 
        int c,counter=0;
        for (c = 0; c < (*box_ptr_ptr)->nodeCount; c++)
        {
            if ((*box_ptr_ptr)->masterNodeList[c].nodeType == 0)
            {
                counter++;
            }
        }
        ierr = PetscPrintf(PETSC_COMM_WORLD,"tot couple node things = %d\n", counter);CHKERRQ(ierr);

        PetscInt i=0;
        for (i = 0; i < 10; i++)
        {
            Couple *cpl = &((*box_ptr_ptr)->masterCoupleList[i]);
            ierr = PetscPrintf(PETSC_COMM_WORLD,"couple %d has node %d and %d\n",i,cpl->nodeID[0],cpl->nodeID[1]);CHKERRQ(ierr);
        }
    }
    ierr = PetscPrintf(PETSC_COMM_WORLD,"g_ptr = %d\n", *gIndex_ptr);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"g = %d\n", gIndex);CHKERRQ(ierr);

    /* produce numbering for internal nodes */
    gIndex = setInternalNodeIndices(*box_ptr_ptr, coupledSystem, gIndex);CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"g_ptr = %d\n", *gIndex_ptr);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"g = %d\n", gIndex);CHKERRQ(ierr);

	/* use final global index to set total internal nodes */
	(*box_ptr_ptr)->nodeInternalCount = gIndex;

	return ierr;
}


PetscErrorCode readInputFile(const char *fileToRead_ptr, Box **box_ptr_ptr, 
                                PetscBool readCouplesOnly, PetscInt *gIndex_ptr, PetscScalar gamma)
{
    PetscErrorCode ierr = 0;
    FILE *fp;
	char line[MAX_LENGTH], *line_ptr;

    /* open file */
    fp = fopen(fileToRead_ptr, "r");
    if (fp == NULL) SETERRQ(PETSC_COMM_WORLD,65,"Error in opening file.");

    /* ignore all entries except for the couple lines */
    while ((line_ptr = fgets(line, sizeof(line), fp)) != NULL)
    {
        if (!readCouplesOnly)   /* if first pass, read everything but don't build couples */
        {
		    ierr = readDataLine(line_ptr, box_ptr_ptr, gIndex_ptr, gamma);CHKERRQ(ierr);
        }
        else                    /* if second pass only read couples and build couple information */
        {
            ierr = readCoupleData(line_ptr, *box_ptr_ptr, gIndex_ptr);CHKERRQ(ierr);
        }
    }
    /* collect file close return code */
    ierr = fclose(fp);
    
    return ierr;
}


void checkInternalNodeIndices(const Box *box_ptr)
{
    PetscInt i;
    for (i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node_ptr = &(box_ptr->masterNodeList[i]);
        if (node_ptr->globalID == -2) 
        {
            PetscPrintf(PETSC_COMM_WORLD,"index is %d\n", i);
            PetscPrintf(PETSC_COMM_WORLD,"node type is: %d\n", node_ptr->nodeType);
        }
        assert(node_ptr->globalID != -2);
    }
}


PetscInt setInternalNodeIndices(Box *box_ptr, const PetscBool coupledSystem, PetscInt coupleCount)
{
    PetscInt totalInternalIndices = 0;
    PetscErrorCode ierr;

    if (coupledSystem)
    {
        PetscInt totalInternalCouples;

        /* coupled numbering */
        totalInternalCouples = setCoupledInternalNodesIndices(box_ptr, coupleCount);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"count after couple stuff %d\n",totalInternalCouples);CHKERRQ(ierr);
        totalInternalIndices = setStandardInternalNodeIndices(box_ptr, totalInternalCouples);
        ierr = PetscPrintf(PETSC_COMM_WORLD,"count after additional stuff %d\n",totalInternalIndices);CHKERRQ(ierr);

        /* if no stray internal nodes are found, count should match last given index */
        if (totalInternalCouples == totalInternalIndices)
        {
            assert(coupleCount == totalInternalIndices);
        }
    }
    else 
    {
        /* standard numbering */
        totalInternalIndices = setStandardInternalNodeIndices(box_ptr, 0);
    }
    checkInternalNodeIndices(box_ptr);

    return totalInternalIndices;
}


PetscInt setStandardInternalNodeIndices(Box *box_ptr, PetscInt newIndex)
{
    PetscInt i;
    PetscErrorCode ierr;
    ierr = PetscPrintf(PETSC_COMM_WORLD,"\nstarting val= %d\n", newIndex);CHKERRQ(ierr);
    /* loop over every node of the network */
    for (i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node_ptr = &(box_ptr->masterNodeList[i]);
        if (node_ptr->globalID == -2)
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Found an additional internal node.\n");CHKERRQ(ierr);

            assert(node_ptr->nodeType == NODE_INTERNAL);
            node_ptr->globalID = newIndex;
            newIndex += 1;
            ierr = PetscPrintf(PETSC_COMM_WORLD,"incremented val = %d\n", newIndex);CHKERRQ(ierr);
        }
    }        

    ierr = PetscPrintf(PETSC_COMM_WORLD,"ending val = %d\n", newIndex);CHKERRQ(ierr);
    return newIndex;
}


PetscInt setCoupledInternalNodesIndices(Box *box_ptr, const PetscInt coupleCount)
{
    PetscInt    i,j,newIndex=0;

    int internalCountInCouples = 0;
    for (i = 0; i < coupleCount; i++)
    {
        PetscBool boundaryCouple = PETSC_FALSE;
        PetscBool allBoundary    = PETSC_TRUE;
        Couple *couple_ptr = &(box_ptr->masterCoupleList[i]);
        
        /* loop over couple in the unlikely case that there is a '3rd' coupled node */
        for (j = 0; j < couple_ptr->nodesInCouple; j++)
        {
            PetscPrintf(PETSC_COMM_WORLD,"%d, ", couple_ptr->nodeID[j]);

            /* reference the node with the corresponding ID */
            Node *node_ptr = &(box_ptr->masterNodeList[couple_ptr->nodeID[j]]);

            /* sanity check: should always match by definition */
            assert(node_ptr->nodeID == couple_ptr->nodeID[j]);

            /* shouldn't encounter any dangling nodes in couples */
            assert(node_ptr->nodeType != 1);

            if (node_ptr->nodeType == 0)
            {
                allBoundary = PETSC_FALSE;
                /* shouldn't assign new ID to anything but new internal nodes */
                assert(node_ptr->globalID == -2);

                /* reassign internal node ID once happy it is the right node */
                node_ptr->globalID = newIndex;

                internalCountInCouples++;
            }
            else if (node_ptr->nodeType == 2)
            {
                /* found boundary couple */
                assert(node_ptr->globalID == -1);
                /* now obsolete */
                /*
                 * boundaryCouple = PETSC_TRUE;
                 * PetscPrintf(PETSC_COMM_WORLD,"\nSkipping boundary couple.\n");
                 * PetscPrintf(PETSC_COMM_WORLD,"Couple is %d: %d-%d\n\n", i, couple_ptr->nodeID[0], couple_ptr->nodeID[j]);
                 * node_ptr->globalID = -1;
                 * //break;
                 */
            }

        }
        /* only increment index when couple is composed of internal nodes */
        //if (!boundaryCouple) newIndex += 1;
        //newIndex += 1;

        if (allBoundary)
        {
            PetscErrorCode ierr;
            ierr = PetscPrintf(PETSC_COMM_WORLD,"Found an all boundary fibre.\n");CHKERRQ(ierr);
        }
        else
        {
            newIndex += 1;
        }

        /* WARNING: Uncaught possibility when all nodes in couple are boundary! */
    }

    PetscInt ierr;
    ierr = PetscPrintf(PETSC_COMM_WORLD,"\nTotal internal couple nodes = %d\n", internalCountInCouples);CHKERRQ(ierr);
    return newIndex;
}


/* Reads network data from a given line pointer */
PetscErrorCode readDataLine(char *line_ptr, Box **box_ptr_ptr, PetscInt *cIndex_ptr, PetscScalar gamma)
{
	PetscErrorCode  ierr = 0;
    PetscBool       coupledSystem = PETSC_FALSE;

	/* collect initial character and move pointer to where cropped line begins */
    char *tkn_ptr        = strtok(line_ptr, " ");
    char *lineCrop_ptr   = tkn_ptr + 2;
	
	/* switch over different line types */
	switch (*tkn_ptr)
	{
		case 'b':
			/* pass line pointer to box line reader */
			ierr = readBoxLine(lineCrop_ptr, box_ptr_ptr);CHKERRQ(ierr);
			break;
		case 'f':
			/* pass line pointer to fibre line reader */
			ierr = readFibreLine(lineCrop_ptr, *box_ptr_ptr);CHKERRQ(ierr);
			break;
		case 'n':
			/* pass line pointer to node line reader */
			ierr = readNodeLine(lineCrop_ptr, *box_ptr_ptr, gamma);CHKERRQ(ierr);
			break;
        case 'c':
            coupledSystem = PETSC_TRUE;
            *cIndex_ptr += 1;
            break;
		default:
			SETERRQ(PETSC_COMM_WORLD,63,"Error in identifying line type. Line size may be insufficient.");
	}

	return ierr;
}


/* Reads box information from a given line pointer */
PetscErrorCode readCoupleData(char *line_ptr, Box *box_ptr, PetscInt *cCount)
{
    PetscErrorCode  ierr = 0;

	/* collect initial character and move pointer to where cropped line begins */
    char *tkn_ptr        = strtok(line_ptr, " ");
    char *lineCrop_ptr   = tkn_ptr + 2;
	
	/* switch over different line types */
	switch (*tkn_ptr)
	{
		case 'b':
		case 'f':
		case 'n':
			break;
        case 'c':
	        /* read in line and increment line number */
            ierr = readCoupleLine(lineCrop_ptr, box_ptr, *cCount);CHKERRQ(ierr);
            *cCount += 1;
            break;
		default:
			SETERRQ(PETSC_COMM_WORLD,63,"Error in identifying line type. Line size may be insufficient.");
	}

    return ierr;
}


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
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, PetscScalar gamma)
{
	PetscErrorCode 	ierr;
  	PetscInt 		nID, nType;
	PetscScalar 	x, y, z;

	/* read in a box line */
	sscanf(line_ptr, "%d %d %lf %lf %lf", &nID, &nType, &x, &y, &z);

	/* assign scanned values to a node */
	ierr = makeNode(box_ptr, nID, nType, x, y, z, gamma);CHKERRQ(ierr);

	return ierr;
}


/* Reads node couple information from a given line pointer */
PetscErrorCode readCoupleLine(char *line_ptr, Box *box_ptr, const PetscInt coupleID)
{
	PetscErrorCode 	ierr = 0;

/* START: temporary read-in of variable node on couple numbers */
	/* preprocess line by removing rhs trailing whitespace */
	char *trimmedLine_ptr = trimRightWhitespace(line_ptr);

    int arraySize  = strlen(trimmedLine_ptr) + 1;
	
	/* declare array to store tokens of fibre information */
	char *cplInfoArray[strlen(trimmedLine_ptr)+1];

	/* split string once and store token */
	char *tkn_ptr = strtok(trimmedLine_ptr, " ");

	PetscInt splitCounter = 0;
	while(tkn_ptr != NULL)	/* while token exists */
	{
		/* add token to array and split string again */
		cplInfoArray[splitCounter] = tkn_ptr;
		tkn_ptr = strtok(NULL, " ");
		splitCounter += 1;
	}

    assert(arraySize > splitCounter);

	/* total nodes is total splits -0 
	 * ...where total splits includes split at end of line     */
	PetscInt nodesOnCouple 	= splitCounter;
    assert(nodesOnCouple < MAX_NODES_ON_COUPLE);
/* END: temporary read-in of variable node on couple numbers */

/* START: temporary makeCouple w/ variable couple length */
    Couple *couple_ptr = &(box_ptr->masterCoupleList[coupleID]);
    couple_ptr->coupleID = coupleID;
    couple_ptr->nodesInCouple = nodesOnCouple;
	PetscInt nID;

	for (nID = 0; nID < splitCounter; nID++)
	{
        assert(nID < MAX_NODES_ON_COUPLE);
        couple_ptr->nodeID[nID] = atoi(cplInfoArray[nID]);
	}
/* END: temporary makeCouple w/ variable couple length */

  	//PetscInt 		nID1, nID2;

	/* read in a node couple line */
	//sscanf(line_ptr, "%d %d", &nID1, &nID2);

	/* assign scanned values to a node */
	//ierr = makeCouple(box_ptr, coupleID, nID1, nID2);CHKERRQ(ierr);

    /* need to implement updated couple builder */
	//ierr = makeCouple(box_ptr, coupleID, nodesInCouple, cplInfoArray);CHKERRQ(ierr);

    /*
    PetscInt i=0;
    Couple *cpl = &(box_ptr->masterCoupleList[i]);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"couple %d has node %d and %d\n",i,cpl->nodeID[0],cpl->nodeID[1]);CHKERRQ(ierr);
    */

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
