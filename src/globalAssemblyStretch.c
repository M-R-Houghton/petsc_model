#include "globalAssemblyStretch.h"


/* Checks stretch matrix contribution indexes are all legal */
void checkMatStretchContIndexes( const PetscInt gIdx_A, const PetscInt gIdx_B, const PetscInt lIdx_A, const PetscInt lIdx_B )
{
	assert(gIdx_A >= 0 && gIdx_B >= 0);     /* protect from unassigned negative global indexing */
	assert(lIdx_A == 0 || lIdx_A == 1);		/* protect from indexing out of range */
	assert(lIdx_B == 0 || lIdx_B == 1);
}


/* Adds a single stretching contribution of 9 values to the global matrix potentially more efficiently */
PetscErrorCode addMatSingleStretchContFAST( Mat globalMat_H, PetscScalar (* const localMat)[6], const PetscInt N,
											const PetscInt gIdx_A, const PetscInt gIdx_B, 
                                            const PetscInt lIdx_A, const PetscInt lIdx_B )
{
    // TODO: Understand why this function doesn't work as expected
	PetscErrorCode 	ierr = 0;
	PetscInt 		row;
	PetscInt 		col[DIMENSION];
	PetscScalar 	val[DIMENSION];
	PetscInt 		i,j;

	checkMatStretchContIndexes(gIdx_A, gIdx_B, lIdx_A, lIdx_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			row    = gIdx_A + i*N;
			col[j] = gIdx_B + j*N;
			val[j] = localMat[lIdx_A + 2*i][lIdx_B + 2*j];
		}
		ierr = MatSetValues(globalMat_H, 1, &row, 3, col, val, ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds a single stretching contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleStretchCont( Mat globalMat_H, PetscScalar (* const localMat)[6], const PetscInt N,
										const PetscInt gIdx_A, const PetscInt gIdx_B, 
                                        const PetscInt lIdx_A, const PetscInt lIdx_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i,j;

	checkMatStretchContIndexes(gIdx_A, gIdx_B, lIdx_A, lIdx_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			ierr = MatSetValue(globalMat_H, gIdx_A + i*N, gIdx_B + j*N, localMat[lIdx_A + 2*i][lIdx_B + 2*j], ADD_VALUES);CHKERRQ(ierr);
			/* WARNING: For debugging ONLY */
			//ierr = PetscPrintf(PETSC_COMM_WORLD,"[CONT] %0.16g\t", localMat[lIdx_A + 2*i][lIdx_B + 2*j]);CHKERRQ(ierr);
		}
		//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);
	}
	//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);

	return ierr;
}


/* Checks stretch vector contribution indexes are all legal */
void checkVecStretchContIndexes( const PetscInt gIdx_A, const PetscInt lIdx_A )
{
	assert(gIdx_A >= 0);
	assert(lIdx_A == 0 || lIdx_A == 1);		/* protect from indexing out of range */
}


/* Adds a single stretching contribution of 3 values to the global vector */
PetscErrorCode addVecSingleStretchCont( Vec globalVec_B, const PetscScalar localVec[], const PetscInt N,
										const PetscInt gIdx_A, const PetscInt lIdx_A )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i;

	checkVecStretchContIndexes(gIdx_A, lIdx_A);

	for (i = 0; i < DIMENSION; i++)
	{
		ierr = VecSetValue(globalVec_B, gIdx_A + i*N, localVec[lIdx_A + 2*i], ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds local stretching contributions to the global matrix and RHS vector */
PetscErrorCode addStretchContToGlobal( Mat globalMat_H, Vec globalVec_B, const PetscInt N,
                                        const PetscInt alph_gID, const PetscInt beta_gID,
                                        const PetscInt alph_nType, const PetscInt beta_nType,
									   	PetscScalar (* const localMat)[6], const PetscScalar localVec[] )
{
	PetscErrorCode ierr = 0;

	const PetscInt alph_lID = 0, beta_lID = 1;		/* setup local IDs */

	if (alph_nType != NODE_DANGLING && 
		beta_nType != NODE_DANGLING)
	{
		/*
		 * add matrix contributions
		 */
		if (alph_nType == NODE_INTERNAL)
		{
			/* add (alpha,alpha) matrix contributions */
			addMatSingleStretchCont(globalMat_H, localMat, N, alph_gID, alph_gID, alph_lID, alph_lID);
		}

		if (beta_nType == NODE_INTERNAL)
		{
			/* add (beta,beta) matrix contributions */
			addMatSingleStretchCont(globalMat_H, localMat, N, beta_gID, beta_gID, beta_lID, beta_lID);
		}

        if (alph_nType == NODE_INTERNAL && 
			beta_nType == NODE_INTERNAL)
		{
			/* add (alpha,beta) and (beta,alpha) mixed matrix contributions */
			addMatSingleStretchCont(globalMat_H, localMat, N, alph_gID, beta_gID, alph_lID, beta_lID);
			addMatSingleStretchCont(globalMat_H, localMat, N, beta_gID, alph_gID, beta_lID, alph_lID);
		}

		/*
		 * add vector contributions
		 */
		if (alph_nType == NODE_BOUNDARY && 
			beta_nType == NODE_INTERNAL)
		{
			/* add (beta) vector contributions */
			addVecSingleStretchCont(globalVec_B, localVec, N, beta_gID, beta_lID);
		}

		if (alph_nType == NODE_INTERNAL && 
			beta_nType == NODE_BOUNDARY)
		{
			/* add (alpha) vector contributions */
			addVecSingleStretchCont(globalVec_B, localVec, N, alph_gID, alph_lID);
		}
	}

	return ierr;
}
