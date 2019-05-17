#include "globalAssemblyStretch.h"


/* Checks stretch matrix contribution indexes are all legal */
void checkMatStretchContIndexes( const PetscInt gInd_A, const PetscInt gInd_B, const PetscInt lInd_A, const PetscInt lInd_B )
{
	assert(gInd_A >= 0 && gInd_B >= 0);     /* protect from unassigned negative global indexing */
	assert(lInd_A == 0 || lInd_A == 1);		/* protect from indexing out of range */
	assert(lInd_B == 0 || lInd_B == 1);
}


/* Adds a single stretching contribution of 9 values to the global matrix potentially more efficiently */
PetscErrorCode addMatSingleStretchContFAST( Mat globalMat_H, const PetscScalar localMat[][6], const PetscInt N,
											const PetscInt gInd_A, const PetscInt gInd_B, 
                                            const PetscInt lInd_A, const PetscInt lInd_B )
{
    // TODO: Understand why this function doesn't work as expected
	PetscErrorCode 	ierr = 0;
	PetscInt 		row;
	PetscInt 		col[DIMENSION];
	PetscScalar 	val[DIMENSION];
	PetscInt 		i,j;

	checkMatStretchContIndexes(gInd_A, gInd_B, lInd_A, lInd_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			row    = gInd_A + i*N;
			col[j] = gInd_B + j*N;
			val[j] = localMat[lInd_A + 2*i][lInd_B + 2*j];
		}
		ierr = MatSetValues(globalMat_H, 1, &row, 3, col, val, ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds a single stretching contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleStretchCont( Mat globalMat_H, const PetscScalar localMat[][6], const PetscInt N,
										const PetscInt gInd_A, const PetscInt gInd_B, 
                                        const PetscInt lInd_A, const PetscInt lInd_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i,j;

	checkMatStretchContIndexes(gInd_A, gInd_B, lInd_A, lInd_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			ierr = MatSetValue(globalMat_H, gInd_A + i*N, gInd_B + j*N, localMat[lInd_A + 2*i][lInd_B + 2*j], ADD_VALUES);CHKERRQ(ierr);
			/* WARNING: For debugging ONLY */
			//ierr = PetscPrintf(PETSC_COMM_WORLD,"[CONT] %0.16g\t", localMat[lInd_A + 2*i][lInd_B + 2*j]);CHKERRQ(ierr);
		}
		//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);
	}
	//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);

	return ierr;
}


/* Checks stretch vector contribution indexes are all legal */
void checkVecStretchContIndexes( const PetscInt gInd_A, const PetscInt lInd_A )
{
	assert(gInd_A != -1);
	assert(lInd_A ==  0 || lInd_A == 1);		/* protect from indexing out of range */
}


/* Adds a single stretching contribution of 3 values to the global vector */
PetscErrorCode addVecSingleStretchCont( Vec globalVec_B, const PetscScalar localVec[], const PetscInt N,
										const PetscInt gInd_A, const PetscInt lInd_A )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i;

	checkVecStretchContIndexes(gInd_A, lInd_A);

	for (i = 0; i < DIMENSION; i++)
	{
		ierr = VecSetValue(globalVec_B, gInd_A + i*N, localVec[lInd_A + 2*i], ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds local stretching contributions to the global matrix and RHS vector */
PetscErrorCode addStretchContToGlobal( const Node *alph_ptr, const Node *beta_ptr,
										Mat globalMat_H, Vec globalVec_B, const PetscInt N,
									   	const PetscScalar localMat[][6], const PetscScalar localVec[] )
{
	PetscErrorCode ierr = 0;

	const PetscInt alph_gID = alph_ptr->globalID;		/* setup global IDs */
	const PetscInt beta_gID = beta_ptr->globalID;

	const PetscInt alph_lID = 0, beta_lID = 1;		/* setup local IDs */

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
