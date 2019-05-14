#include "networkBuild.h"

/* Initiates network build routine */
PetscErrorCode networkBuild()
{
    /* TODO: This function is obsolete and will be removed */
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Building network objects...\n");CHKERRQ(ierr);

	return ierr;
}


/* Creates a parameters structure */
Parameters *makeParameters(const char *input, const char *output, const PetscScalar gamma, const PetscScalar yMod)
{
	/* allocate memory */
	Parameters *par_ptr = (Parameters *)malloc(sizeof(Parameters));

	/* assign attributes */
	strcpy(par_ptr->inputNetwork, input);
	strcpy(par_ptr->outputNetwork, output);
	par_ptr->gamma = gamma;
	par_ptr->youngsModulus = yMod;
    // TODO: Check input and output do not exceed 100 chars

	/* should not be assigned by user */
	par_ptr->energyStre = 0;
	par_ptr->energyBend = 0;
	par_ptr->energyTotl = 0;
	par_ptr->energyAffn = 0;
	par_ptr->shearModulus = 0;
	par_ptr->shearModAffn = 0;

	return par_ptr;
}


/* Creates a sparse structure */
Sparse *makeSparse(PetscInt n, PetscInt nz)
{
	/* check for invalid arguments given */
	assert(n >= nz);

	/* allocate memory */
	Sparse *sparse_ptr = (Sparse *)malloc(sizeof(Sparse));

	/* assign attributes */
	sparse_ptr->n = n;
	sparse_ptr->nz = nz;
	sparse_ptr->counter = (PetscInt *)calloc(sparse_ptr->n,sizeof(PetscInt));
	sparse_ptr->rowp = (PetscInt *)calloc(sparse_ptr->n+1,sizeof(PetscInt));
	sparse_ptr->col = (PetscInt *)calloc(sparse_ptr->nz,sizeof(PetscInt));
	sparse_ptr->mat = (PetscScalar *)calloc(sparse_ptr->nz,sizeof(PetscScalar));

	return sparse_ptr;
}


/* Checks box arguments are all legal */
void checkBoxArguments(PetscInt nCount, PetscInt fCount,
							PetscScalar xDim, PetscScalar yDim, PetscScalar zDim,
							PetscInt xPer, PetscInt yPer, PetscInt zPer)
{
	assert(nCount >= 0);
	assert(fCount >= 0);

	assert(xDim >= 0);
	assert(yDim >= 0);
	assert(zDim >= 0);

	assert(xPer >= 0);
	assert(yPer >= 0);
	assert(zPer >= 0);

	assert(xPer <= 1);
	assert(yPer <= 1);
	assert(zPer <= 1);
}


/* Creates a box structure */
Box *makeBox(PetscInt nCount, PetscInt fCount,
				PetscScalar xDim, PetscScalar yDim, PetscScalar zDim,
				PetscInt xPer, PetscInt yPer, PetscInt zPer)
{
	/* validate arguments */
	checkBoxArguments(nCount,fCount,xDim,yDim,zDim,xPer,yPer,zPer);

	/* allocate memory */
	Box *box_ptr = (Box *)malloc(sizeof(Box));

	/* assign attributes */
	box_ptr->nodeCount = nCount;
	box_ptr->nodeInternalCount = -1;
	box_ptr->fibreCount = fCount;
	box_ptr->xyzDimension[0] = xDim;
	box_ptr->xyzDimension[1] = yDim;
	box_ptr->xyzDimension[2] = zDim;
	box_ptr->xyzPeriodic[0] = xPer;
	box_ptr->xyzPeriodic[1] = yPer;
	box_ptr->xyzPeriodic[2] = zPer;

	/* assign allocated memory to master node/fibre lists */
	box_ptr->masterNodeList = (Node*)calloc(nCount, sizeof(Node));
	box_ptr->masterFibreList = (Fibre*)calloc(fCount, sizeof(Fibre));

    /* couple count should only be changed from initial val if system is coupled */
    box_ptr->coupleCount = 0;
    /* couple list memory should be allocated after no. of couples is known */
    box_ptr->masterCoupleList = NULL;

	return box_ptr;
}


/* Checks fibre arguments are all legal */
void checkFibreArguments(Box *box_ptr, PetscInt fID, PetscInt nOnFibre,
				PetscScalar radius, Node **nList_ptr_ptr)
{
	assert(box_ptr       != NULL);
	assert(nList_ptr_ptr != NULL);

	assert(fID      >= 0);
	assert(nOnFibre >= 0);
	assert(radius   >= 0);
}


/* Creates a fibre within its allocated location in a box */
PetscErrorCode makeFibre(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, PetscScalar radius, Node **nList_ptr_ptr)
{
	PetscErrorCode ierr = 0;

	/* validate arguments */
	checkFibreArguments(box_ptr, fID, nOnFibre, radius, nList_ptr_ptr);

	/* assign attributes */
	box_ptr->masterFibreList[fID].fibreID = fID;
	box_ptr->masterFibreList[fID].nodesOnFibre = nOnFibre;
	box_ptr->masterFibreList[fID].radius = radius;
    box_ptr->masterFibreList[fID].nodesOnFibreList = nList_ptr_ptr;

    /* additional attributes not to be assigned by user */
    box_ptr->masterFibreList[fID].fibreStreEnergy = 0;
    box_ptr->masterFibreList[fID].fibreBendEnergy = 0;
    box_ptr->masterFibreList[fID].fibreAffnEnergy = 0;

    return ierr;
}


/* Checks node arguments are all legal */
void checkNodeArguments(Box *box_ptr, PetscInt nID, PetscInt nType,
				PetscScalar x, PetscScalar y, PetscScalar z, PetscScalar gamma)
{
	assert(box_ptr != NULL);
	assert(nID >= 0);
	assert(nType == NODE_INTERNAL || nType == NODE_BOUNDARY || nType == NODE_DANGLING);
}


/* Creates a node within its allocated location in a box */
PetscErrorCode makeNode(Box *box_ptr, PetscInt nID, PetscInt nType,
				PetscScalar x, PetscScalar y, PetscScalar z, PetscScalar gamma)
{
	PetscErrorCode ierr = 0;

	/* validate arguments */
	checkNodeArguments(box_ptr, nID, nType, x, y, z, gamma);

	/* create shortcut for readability */
	Node *node_ptr = &(box_ptr->masterNodeList[nID]);

	/* assign attributes */
	node_ptr->nodeID = nID;
	node_ptr->nodeType = nType;
	node_ptr->xyzCoord[0] = x;
	node_ptr->xyzCoord[1] = y;
	node_ptr->xyzCoord[2] = z;

	/* default before switch case */
	node_ptr->globalID = -1;
	node_ptr->xyzDisplacement[0] = 0;
	node_ptr->xyzDisplacement[1] = 0;
	node_ptr->xyzDisplacement[2] = 0;

	/* assign affine displacements like boundary nodes */
	node_ptr->xyzAffDisplacement[0] = gamma * y;
	node_ptr->xyzAffDisplacement[1] = 0;
	node_ptr->xyzAffDisplacement[2] = 0; 	/* is node_ptr->xyzCoord[1] safer than y? */

	/* NOTE: node displacement assumes boundary nodes are fixed at y=0...	*
	 *		...and sheared at y=HEIGHT along the x-axis 					*/

	switch (nType)
	{
		case NODE_INTERNAL:
			/* add global ID */
            node_ptr->globalID = -2;
			break;
		case NODE_BOUNDARY:
			/* apply boundary conditions */
			node_ptr->xyzDisplacement[0] = gamma * y;
/*		 	node_ptr->xyzDisplacement[1] = 0; *****	*
 * 		 	node_ptr->xyzDisplacement[2] = 0; *****	*/
			break;
		case NODE_DANGLING:
			/* do nothing at the moment */
			break;
		default:
			SETERRQ(PETSC_COMM_WORLD,63,"Error in identifying node type");
	}

	return ierr;
}


/* Checks couple arguments are all legal */
void checkCoupleArguments(Box *box_ptr, PetscInt const cID, PetscInt const nID1, PetscInt const nID2)
{
    assert(box_ptr != NULL);
    assert(cID >= 0);
    assert(nID1 >= 0 && nID2 >= 0);
}


PetscErrorCode makeCouple(Box *box_ptr, const PetscInt coupleID, const PetscInt nodesOnCouple, const PetscInt *nodeIDList)
{
    PetscErrorCode ierr = 0;

    /* TODO: move this into checkCoupleArguments */
    assert(nodesOnCouple < MAX_NODES_ON_COUPLE);

    Couple *couple_ptr = &(box_ptr->masterCoupleList[coupleID]);
    couple_ptr->coupleID = coupleID;
    couple_ptr->nodesInCouple = nodesOnCouple;

    PetscInt i;
    for (i = 0; i < nodesOnCouple; i++)
    {
        couple_ptr->nodeID[i] = nodeIDList[i];
    }

    return ierr;
}


