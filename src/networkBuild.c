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
	/* exit function if invalid arguments given */
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
PetscErrorCode checkBoxArguments(PetscInt nCount, PetscInt fCount, 
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


	return 0;
}


/* Creates a box structure */
Box *makeBox(PetscInt nCount, PetscInt fCount, 
				PetscScalar xDim, PetscScalar yDim, PetscScalar zDim, 
				PetscInt xPer, PetscInt yPer, PetscInt zPer)
{
	checkBoxArguments(nCount,fCount,xDim,yDim,zDim,xPer,yPer,zPer);

	/* do some stuff here */
	
	return NULL;
}

