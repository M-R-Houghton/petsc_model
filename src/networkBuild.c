#include "networkBuild.h"

/* Initiates network build routine */
PetscErrorCode networkBuild()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Building network objects...\n");CHKERRQ(ierr);

	return ierr;
}


/* Creates a parameters structure */
Parameters *makeParameters(PetscScalar gamma, PetscScalar yMod)
{
	/* allocate memory */
	Parameters *par_ptr = (Parameters *)malloc(sizeof(Parameters));

	/* assign attributes */
	par_ptr->gamma = gamma;
	par_ptr->youngsModulus = yMod;

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

	assert(xPer < 2);
	assert(yPer < 2);
	assert(zPer < 2);
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

	return box_ptr;
}


/* Checks fibre arguments are all legal */
void checkFibreArguments(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, 
				PetscScalar radius, Node **nList_ptr_ptr)
{
	assert(box_ptr != NULL);
	assert(fID >= 0);
	assert(nOnFibre >= 0);
	assert(radius >= 0);
	assert(nList_ptr_ptr != NULL);
}


/* Creates a fibre within its allocated location in a box */
void makeFibre(Box *box_ptr, PetscInt fID, PetscInt nOnFibre, PetscScalar radius, Node **nList_ptr_ptr) 
{
	/* validate arguments */
	checkFibreArguments(box_ptr, fID, nOnFibre, radius, nList_ptr_ptr);

	/* assign attributes */
	box_ptr->masterFibreList[fID].fibreID = fID;
	box_ptr->masterFibreList[fID].nodesOnFibre = nOnFibre;
	box_ptr->masterFibreList[fID].radius = radius;
    box_ptr->masterFibreList[fID].nodesOnFibreList = nList_ptr_ptr;
}


/* Checks node arguments are all legal */
void checkNodeArguments(Box *box_ptr, PetscInt nID, PetscInt nType, 
				PetscScalar x, PetscScalar y, PetscScalar z, PetscInt *gIndex_ptr, PetscScalar gamma)
{
	assert(box_ptr != NULL);
	assert(nID >= 0);
	assert(nType == NODE_INTERNAL || nType == NODE_BOUNDARY || nType == NODE_DANGLING);
	assert(gIndex_ptr != NULL);
}


/* Creates a node within its allocated location in a box */
PetscErrorCode makeNode(Box *box_ptr, PetscInt nID, PetscInt nType, 
				PetscScalar x, PetscScalar y, PetscScalar z, PetscInt *gIndex_ptr, PetscScalar gamma)
{
	PetscErrorCode ierr = 0;
	
	/* validate arguments */
	checkNodeArguments(box_ptr, nID, nType, x, y, z, gIndex_ptr, gamma);

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

	switch (nType)
	{
		case NODE_INTERNAL:
			/* add global ID */
			node_ptr->globalID = *gIndex_ptr;
			*gIndex_ptr += 1;
			break;
		case NODE_BOUNDARY:
			/* apply boundary conditions */
			node_ptr->xyzDisplacement[0] = gamma * node_ptr->xyzCoord[1];
			node_ptr->xyzDisplacement[1] = 0;
			node_ptr->xyzDisplacement[2] = 0;
			break;
		case NODE_DANGLING:
			/* do nothing at the moment */
			break;
		default:
			SETERRQ(PETSC_COMM_WORLD,63,"Error in identifying node type");
	}

	return ierr;
}






