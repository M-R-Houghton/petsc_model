#include "globalAssemblyStretch.h"


/* Adds a single stretching contribution of 9 values to the global matrix potentially more efficiently */
PetscErrorCode addSingleStretchContFAST( Box *box_ptr, Mat globalMat_H, PetscScalar localMat[][6], 
											PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		N 	 = box_ptr->nodeInternalCount;
	PetscInt 		row;
	PetscInt 		col[DIMENSION];
	PetscScalar 	val[DIMENSION];
	PetscInt 		i,j;

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			row    = gInd_A + i*N;
			col[j] = gInd_B + j*N;
			val[j] = localMat[lInd_A + 2*i][lInd_B + 2*j];
		}
		ierr = MatSetValues(globalMat_H, 1, &row, 3, col, val, INSERT_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds a single stretching contribution of 9 values to the global matrix */
PetscErrorCode addSingleStretchCont( Box *box_ptr, Mat globalMat_H, PetscScalar localMat[][6], 
										PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		N 	 = box_ptr->nodeInternalCount;
	PetscInt 		i,j;

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			//ierr = MatSetValue(globalMat_H, gInd_A + i*N, gInd_B + j*N, localMat[lInd_A + 2*i][lInd_B + 2*j], INSERT_VALUES);CHKERRQ(ierr);
		}
	}

	return ierr;
}


/* Adds local stretching contributions to the global matrix and RHS vector */
PetscErrorCode addStretchContToGlobal( Node *alph_ptr, Node *beta_ptr,
										Mat globalMat_H, Vec globalVec_B, 
									   	PetscScalar localMat[][6], PetscScalar localVec[] )
{
	PetscErrorCode ierr = 0;

	/*
	 * add matrix contributions
	 */
	if (alph_ptr->nodeType == NODE_INTERNAL)
	{
		/* add (alpha,alpha) matrix contributions */
	}

	if (beta_ptr->nodeType == NODE_INTERNAL)
	{
		/* add (beta,beta) matrix contributions */
	}

	if (alph_ptr->nodeType == NODE_INTERNAL && 
		beta_ptr->nodeType == NODE_INTERNAL)
	{
		/* add (alpha,beta) and (beta,alpha) mixed matrix contributions */
	}


	/*
	 * add vector contributions
	 */
	if (alph_ptr->nodeType == NODE_BOUNDARY && 
		beta_ptr->nodeType == NODE_INTERNAL)
	{
		/* add (beta) vector contributions */
	}

	if (alph_ptr->nodeType == NODE_INTERNAL && 
		beta_ptr->nodeType == NODE_BOUNDARY)
	{
		/* add (alpha) vector contributions */
	}


	return ierr;
}