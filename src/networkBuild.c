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
	if (n < nz) return NULL;

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
PetscInt checkBoxArguments(PetscInt nCount, PetscInt fCount, 
							PetscScalar xDim, PetscScalar yDim, PetscScalar zDim, 
							PetscInt xPer, PetscInt yPer, PetscInt zPer)
{
	if (nCount < 0 || fCount < 0)
	{
		return -1;
	}
	else if (xDim < 0 || yDim < 0 || zDim < 0)
	{
		return -1;
	}
	else if (xPer < 0 || yPer < 0 || zPer < 0)
	{
		return -1;
	}
	else if (xPer > 1 || yPer > 1 || zPer > 1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


/* Creates a box structure */
Box *makeBox(PetscInt nCount, PetscInt fCount, 
				PetscScalar xDim, PetscScalar yDim, PetscScalar zDim, 
				PetscInt xPer, PetscInt yPer, PetscInt zPer)
{
	if (checkBoxArguments(nCount,fCount,xDim,yDim,zDim,xPer,yPer,zPer) == -1) return NULL;
	
	return NULL;
}

