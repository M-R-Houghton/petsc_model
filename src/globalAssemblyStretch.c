#include "globalAssemblyStretch.h"


/* Adds a single stretching contribution of 9 values to the global matrix potentially more efficiently */
PetscErrorCode addMatSingleStretchContFAST( Mat globalMat_H, PetscScalar localMat[][6], PetscInt N,
											PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B )
{
	PetscErrorCode 	ierr = 0;
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
PetscErrorCode addMatSingleStretchCont( Mat globalMat_H, PetscScalar localMat[][6], PetscInt N,
										PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i,j;

	assert(gInd_A != -1 && gInd_B != -1);
	assert(lInd_A ==  0 || lInd_A ==  1);		/* protect from indexing out of range */
	assert(lInd_B ==  0 || lInd_B ==  1);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			ierr = MatSetValue(globalMat_H, gInd_A + i*N, gInd_B + j*N, localMat[lInd_A + 2*i][lInd_B + 2*j], INSERT_VALUES);CHKERRQ(ierr);
		}
	}

	return ierr;
}


/* Adds a single stretching contribution of 3 values to the global vector */
PetscErrorCode addVecSingleStretchCont( Vec globalVec_B, PetscScalar localVec[], PetscInt N,
										PetscInt gInd_A, PetscInt lInd_A )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i;

	assert(gInd_A != -1);
	assert(lInd_A ==  0 || lInd_A == 1);		/* protect from indexing out of range */

	for (i = 0; i < DIMENSION; i++)
	{
		ierr = VecSetValue(globalVec_B, gInd_A + i*N, localVec[lInd_A + 2*i], INSERT_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds local stretching contributions to the global matrix and RHS vector */
PetscErrorCode addStretchContToGlobal( Node *alph_ptr, Node *beta_ptr,
										Mat globalMat_H, Vec globalVec_B, PetscInt N,
									   	PetscScalar localMat[][6], PetscScalar localVec[] )
{
	PetscErrorCode ierr = 0;

	PetscInt alph_gID = alph_ptr->globalID;		/* setup global IDs */
	PetscInt beta_gID = beta_ptr->globalID;

	PetscInt alph_lID = 0, beta_lID = 1;		/* setup local IDs */

	if (alph_ptr->nodeType != NODE_DANGLING && 
		beta_ptr->nodeType != NODE_DANGLING)
	{
		/*
		 * add matrix contributions
		 */
		if (alph_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (alpha,alpha) matrix contributions */
			addMatSingleStretchCont(globalMat_H, localMat, N, alph_gID, alph_gID, alph_lID, alph_lID);
		}

		if (beta_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (beta,beta) matrix contributions */
			addMatSingleStretchCont(globalMat_H, localMat, N, beta_gID, beta_gID, beta_lID, beta_lID);
		}

		if (alph_ptr->nodeType == NODE_INTERNAL && 
			beta_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (alpha,beta) and (beta,alpha) mixed matrix contributions */
			addMatSingleStretchCont(globalMat_H, localMat, N, alph_gID, beta_gID, alph_lID, beta_lID);
			addMatSingleStretchCont(globalMat_H, localMat, N, beta_gID, alph_gID, beta_lID, alph_lID);
		}

		/*
		 * add vector contributions
		 */
		if (alph_ptr->nodeType == NODE_BOUNDARY && 
			beta_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (beta) vector contributions */
			addVecSingleStretchCont(globalVec_B, localVec, N, beta_gID, beta_lID);
		}

		if (alph_ptr->nodeType == NODE_INTERNAL && 
			beta_ptr->nodeType == NODE_BOUNDARY)
		{
			/* add (alpha) vector contributions */
			addVecSingleStretchCont(globalVec_B, localVec, N, alph_gID, alph_lID);
		}
	}

	return ierr;
}