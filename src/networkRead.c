#include "networkRead.h"

/* Initiates network read in routine */
PetscErrorCode networkRead(const char *fileToRead_ptr, Box **box_ptr_ptr, const PetscScalar gamma)
{
    PetscErrorCode 	ierr = 0;

    /* setup global index */
    PetscInt        coupleCount = 0;

    /* read data line increases the number of couples if there are any */ 
    readInputFile(fileToRead_ptr, box_ptr_ptr, &coupleCount, gamma);

    /* run readInputFile again if system is coupled */
    if (coupleCount > 0)
    {
        /* use counted couples to assign count and allocate master couple array */
        (*box_ptr_ptr)->coupleCount = coupleCount;
        (*box_ptr_ptr)->masterCoupleList = (Couple*)calloc(coupleCount, sizeof(Couple));

        /* this time readInputFile just reads couple lines */
        readInputFile(fileToRead_ptr, box_ptr_ptr, &coupleCount, gamma);

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
    }
    ierr = PetscPrintf(PETSC_COMM_WORLD,"g = %d\n", coupleCount);CHKERRQ(ierr);

    /* produce numbering for internal nodes */
    PetscInt gIDTotal = setInternalNodeIndices(*box_ptr_ptr, coupleCount);CHKERRQ(ierr);

    ierr = PetscPrintf(PETSC_COMM_WORLD,"g = %d\n", coupleCount);CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"gIDTotal = %d\n", gIDTotal);CHKERRQ(ierr);

    /* use final global index to set total internal nodes */
    (*box_ptr_ptr)->nodeInternalCount = gIDTotal;

    return ierr;
}


PetscErrorCode readInputFile(const char *fileToRead_ptr, Box **box_ptr_ptr, 
        PetscInt *coupleCount, const PetscScalar gamma)
{
    PetscErrorCode ierr = 0;
    FILE *fp;
    char line[MAX_LENGTH], *line_ptr;

    PetscBool readCouplesOnly = (*coupleCount > 0) ? PETSC_TRUE : PETSC_FALSE;
    *coupleCount = 0;

    /* open file */
    fp = fopen(fileToRead_ptr, "r");
    if (fp == NULL) SETERRQ(PETSC_COMM_WORLD,PETSC_ERR_FILE_OPEN,"Error in opening file.");

    /* this should always be zero before starting */
    assert(*coupleCount == 0);

    while ((line_ptr = fgets(line, sizeof(line), fp)) != NULL)
    {
        ierr = readDataLine(line_ptr, box_ptr_ptr, readCouplesOnly, coupleCount, gamma);CHKERRQ(ierr);
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


PetscInt setInternalNodeIndices(Box *box_ptr, const PetscInt coupleCount)
{
    PetscErrorCode  ierr;
    PetscInt        nextIndex = 0;

    if (coupleCount > 0)
    {
        /* coupled numbering */
        ierr = setCoupledInternalNodeIndices(box_ptr, coupleCount, &nextIndex);
        assert(coupleCount == nextIndex);
    }
    /* standard numbering */
    ierr = setStandardInternalNodeIndices(box_ptr, &nextIndex);
    assert(coupleCount <= nextIndex);
    checkInternalNodeIndices(box_ptr);

    return nextIndex;
}


PetscInt setStandardInternalNodeIndices(Box *box_ptr, PetscInt *nextIndex)
{
    PetscErrorCode  ierr = 0;
    PetscInt        i;

    for (i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node_ptr = &(box_ptr->masterNodeList[i]);
        if (node_ptr->globalID == -2)
        {
            assert(node_ptr->nodeType == NODE_INTERNAL);
            node_ptr->globalID = *nextIndex;
            *nextIndex += 1;
        }
    }        
    return ierr;
}


PetscInt setCoupledInternalNodeIndices(Box *box_ptr, const PetscInt coupleCount, PetscInt *nextIndex)
{
    PetscErrorCode  ierr = 0;
    PetscInt        i,j;
    PetscInt        internalCountInCouples = 0;

    for (i = 0; i < coupleCount; i++)
    {
        /* assume all nodes on cpl are boundary and find contradiction */
        PetscBool   allBoundary = PETSC_TRUE;
        Couple      *couple_ptr = &(box_ptr->masterCoupleList[i]);

        /* loop over nodes on couple and reassign IDs of any internal nodes */
        for (j = 0; j < couple_ptr->nodesInCouple; j++)
        {
            ierr = PetscPrintf(PETSC_COMM_WORLD,"%d, ", couple_ptr->nodeID[j]);CHKERRQ(ierr);

            /* reference the node with the corresponding ID */
            Node *node_ptr = &(box_ptr->masterNodeList[couple_ptr->nodeID[j]]);

            /* node and cpl IDs should be consistent by definition */
            assert(node_ptr->nodeID == couple_ptr->nodeID[j]);
            /* dangling nodes in couples should not be possible */
            assert(node_ptr->nodeType != 1);

            if (node_ptr->nodeType == 0)
            {
                /* shouldn't assign new ID to anything but new internal nodes */
                assert(node_ptr->globalID == -2);

                /* assign new ID and increment count of internal nodes */
                node_ptr->globalID = *nextIndex;
                internalCountInCouples++;

                allBoundary = PETSC_FALSE;

                /* this assertion means we can do this function in a nicer way */
                assert(node_ptr->globalID == couple_ptr->coupleID);
            }
            else 
            {
                /* any boundary nodes should not have had ID reassigned yet */
                assert(node_ptr->globalID == -1);
            }
        }

        /* only increment index when couple is contains some internal nodes */
        /* increment out of node loop to keep index same for nodes on same cpl */
        if (!allBoundary)
        {
            *nextIndex += 1;
        }
    }

    ierr = PetscPrintf(PETSC_COMM_WORLD,"\nTotal internal couple nodes = %d\n", internalCountInCouples);CHKERRQ(ierr);
    return ierr;
}


/* Reads network data from a given line pointer */
PetscErrorCode readDataLine(char *line_ptr, Box **box_ptr_ptr, const PetscBool readCouplesOnly, PetscInt *coupleCount, const PetscScalar gamma)
{
    PetscErrorCode  ierr = 0;

    /* collect initial character and move pointer to where cropped line begins */
    char *tkn_ptr        = strtok(line_ptr, " ");
    char *lineCrop_ptr   = tkn_ptr + 2;

    if (!readCouplesOnly)
    {
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
                *coupleCount += 1;
                break;
            default:
                SETERRQ(PETSC_COMM_WORLD,PETSC_ERR_ARG_UNKNOWN_TYPE,
                        "Error in identifying line type. Line size may be insufficient.");
        }
    }
    else
    {
        /* switch over different line types */
        switch (*tkn_ptr)
        {
            case 'b':
            case 'f':
            case 'n':
                break;
            case 'c':
                /* pass line pointer to cpl line reader */
                ierr = readCoupleLine(lineCrop_ptr, *box_ptr_ptr, *coupleCount);CHKERRQ(ierr);
                *coupleCount += 1;
                break;
            default:
                SETERRQ(PETSC_COMM_WORLD,PETSC_ERR_ARG_UNKNOWN_TYPE,
                        "Error in identifying line type. Line size may be insufficient.");
        }
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
    PetscScalar fibreInfoArray[strlen(line_ptr)+1];

    /* check allocated info array is large enough to store line_ptr */
    assert(strlen(line_ptr) < sizeof(fibreInfoArray)/sizeof(fibreInfoArray[0]));

    /* tokenise the line and retrieve total count of tokens */
    PetscInt tknCount = tokeniseLine(line_ptr, fibreInfoArray);

    /* total nodes is token count -1 for ID and -1 for radius
     * ...where token count includes a token split at end of line */
    PetscInt nodesOnFibre = tknCount - 2;

    /* determine the first few fibre attributes */
    PetscInt fibreID 		= (int)fibreInfoArray[0];
    PetscScalar radius 		= fibreInfoArray[1];

    /* allocate storage for list of nodes on fibre */
    Node **nodeList_ptr_ptr = (Node**)calloc(nodesOnFibre, sizeof(Node*));

    PetscInt fibreInfoIndex = 2;		/* move past ID and radius */
    PetscInt nodeListIndex  = 0;		/* set counter for node list index */

    /* loop through node IDs in the fibre information array */
    for (fibreInfoIndex = 2; fibreInfoIndex < tknCount; fibreInfoIndex++)
    {
        /* typecast to find node ID */
        PetscInt nodeID = (int)fibreInfoArray[fibreInfoIndex];

        /* store node pointer in node list */
        nodeList_ptr_ptr[nodeListIndex] = &(box_ptr->masterNodeList[nodeID]);

        nodeListIndex += 1;
    }

    /* create a fibre using read-in information */
    ierr = makeFibre(box_ptr, fibreID, nodesOnFibre, radius, nodeList_ptr_ptr);CHKERRQ(ierr);

    return ierr;
}


/* Reads node information from a given line pointer */
PetscErrorCode readNodeLine(char *line_ptr, Box *box_ptr, const PetscScalar gamma)
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


/* Tokenises a line into floats by splitting at whitespace */
PetscInt tokeniseLine(char *line_ptr, PetscScalar *infoArray)
{
    /* WARNING: THIS FUNCTION IS NOT THREAD SAFE */
    PetscInt tknCount = 0;

    /* preprocess line by removing rhs trailing whitespace */
    char *trimmedLine_ptr = trimRightWhitespace(line_ptr);

    /* split string once and store token */
    char *tkn_ptr = strtok(trimmedLine_ptr, " ");

    /* loop until no more tokens can be taken */
    while(tkn_ptr != NULL)
    {
        infoArray[tknCount] = atof(tkn_ptr);
        tkn_ptr = strtok(NULL, " ");
        tknCount += 1;
    }

    return tknCount;
}


/* Reads node couple information from a given line pointer */
PetscErrorCode readCoupleLine(char *line_ptr, Box *box_ptr, const PetscInt coupleID)
{
    PetscErrorCode 	ierr = 0;
    PetscInt        lineLength = strlen(line_ptr)+1;
    PetscScalar     cplInfoArray[lineLength];

    /* check allocated info array is large enough to store line_ptr */
    assert(lineLength <= sizeof(cplInfoArray)/sizeof(cplInfoArray[0]));

    /* tokenise the line and retrieve total count of tokens 
     * ...which in this case is the number of nodes on the couple */
    PetscInt nodesOnCouple = tokeniseLine(line_ptr, cplInfoArray);
    assert(nodesOnCouple < lineLength);

    /* until node ID list is dynamically allocated, the limit should be enforced */
    assert(nodesOnCouple < MAX_NODES_ON_COUPLE);

    /* allocate an array for storing the exact number of node IDs */
    PetscInt *nodeIDList = (PetscInt*)calloc(nodesOnCouple, sizeof(PetscInt*));

    /* loop over each node ID and store it in temporary node ID list */
    PetscInt nIndex;
    for (nIndex = 0; nIndex < nodesOnCouple; nIndex++)
    {
        assert(nIndex < MAX_NODES_ON_COUPLE);

        /* TODO: make sure this cast is safe for large values */
        nodeIDList[nIndex] = (PetscInt)cplInfoArray[nIndex];
    }
    /* NOTE: there is some redundancy here, but for clarity the couple line is...
     * ...first parsed and then sent to a couple builder function, just as the...
     * ...cases of nodes and fibres */

    /* pass the parsed info to the couple builder */
    ierr = makeCouple(box_ptr, coupleID, nodesOnCouple, nodeIDList);CHKERRQ(ierr);

    /* node IDs now read into couple struct so clean up */
    free(nodeIDList);

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
