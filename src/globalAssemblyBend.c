#include "globalAssemblyBend.h"


/* Checks bend matrix contribution indexes are all legal */
void checkMatBendContIndexes( const PetscInt gIdx_A, const PetscInt gIdx_B, 
                                const PetscInt lIdx_A, const PetscInt lIdx_B )
{
    /* protect from unassigned negative global indexing */
	assert(gIdx_A >= 0 && gIdx_B >= 0); 
	/* protect from indexing out of range */
	assert(lIdx_A == 0 || lIdx_A == 1 || lIdx_A == 2);
	assert(lIdx_B == 0 || lIdx_B == 1 || lIdx_B == 2);
}


/* Adds a single bending contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleBendContFAST( Mat globalMat_H, PetscScalar (* const localMat)[9], const PetscInt N,
										 const PetscInt gIdx_A, const PetscInt gIdx_B, 
                                         const PetscInt lIdx_A, const PetscInt lIdx_B )
{
    // TODO: Understand why this function doesn't work as expected
	PetscErrorCode 	ierr = 0;
	PetscInt 		row;
	PetscInt 		col[DIMENSION];
	PetscScalar 	val[DIMENSION];
	PetscInt 		i,j;

	checkMatBendContIndexes(gIdx_A, gIdx_B, lIdx_A, lIdx_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			row    = gIdx_A + i*N;
			col[j] = gIdx_B + j*N;
			val[j] = localMat[lIdx_A + 3*i][lIdx_B + 3*j];
		}
		ierr = MatSetValues(globalMat_H, 1, &row, 3, col, val, ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds a single bending contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleBendCont( Mat globalMat_H, PetscScalar (* const localMat)[9], const PetscInt N,
									 const PetscInt gIdx_A, const PetscInt gIdx_B, 
                                     const PetscInt lIdx_A, const PetscInt lIdx_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i,j;

	checkMatBendContIndexes(gIdx_A, gIdx_B, lIdx_A, lIdx_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			ierr = MatSetValue(globalMat_H, gIdx_A + i*N, gIdx_B + j*N, localMat[lIdx_A + 3*i][lIdx_B + 3*j], ADD_VALUES);CHKERRQ(ierr);
			/* WARNING: For debugging ONLY */
			//ierr = PetscPrintf(PETSC_COMM_WORLD,"[BCONT] %0.16g\t", localMat[lIdx_A + 3*i][lIdx_B + 3*j]);CHKERRQ(ierr);
		}
		//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);
	}
	//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);

	return ierr;
}


/* Checks bend vector contribution indexes are all legal */
void checkVecBendContIndexes( const PetscInt gIdx_A, const PetscInt lIdx_A )
{
	assert(gIdx_A >= 0);
	assert(lIdx_A == 0 || lIdx_A == 1 || lIdx_A == 2);
}


/* Adds a single bending contribution of 3 values to the global vector */
PetscErrorCode addVecSingleBendCont( Vec globalVec_B, const PetscScalar localVec[], 
                                     const PetscInt N, const PetscInt gIdx_A, const PetscInt lIdx_A )
{
	PetscErrorCode ierr = 0;
	PetscInt 		i;

	checkVecBendContIndexes(gIdx_A, lIdx_A);

	for (i = 0; i < DIMENSION; i++)
	{
		ierr = VecSetValue(globalVec_B, gIdx_A + i*N, localVec[lIdx_A + 3*i], ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds local bending contributions to the global matrix and RHS vector */
PetscErrorCode addBendContToGlobal( Mat globalMat_H, Vec globalVec_B, const PetscInt N,
                                    const PetscInt alph_gID, const PetscInt omeg_gID, const PetscInt beta_gID,
                                    const PetscInt alph_nType, const PetscInt omeg_nType, const PetscInt beta_nType,
									PetscScalar (* const localMat)[9], const PetscScalar localVec[] )
{
	PetscErrorCode ierr = 0;

    /* setup local IDs */
	const PetscInt alph_lID = 0;	
	const PetscInt omeg_lID = 1; 
	const PetscInt beta_lID = 2;

	if (omeg_nType == NODE_INTERNAL &&
		alph_nType != NODE_DANGLING &&
		beta_nType != NODE_DANGLING)
	{
		/*
		 * add matrix contributions
		 */

		/* add (omega,omega) matrix contributions */
		ierr = addMatSingleBendCont(globalMat_H, localMat, N, omeg_gID, omeg_gID, omeg_lID, omeg_lID);CHKERRQ(ierr);

		if (alph_nType == NODE_INTERNAL)
		{
			/* add (alpha,alpha) matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, alph_gID, alph_gID, alph_lID, alph_lID);CHKERRQ(ierr);

			/* add (alpha,omega) and (omega,alpha) mixed matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, alph_gID, omeg_gID, alph_lID, omeg_lID);CHKERRQ(ierr);
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, omeg_gID, alph_gID, omeg_lID, alph_lID);CHKERRQ(ierr);
		}

		if (beta_nType == NODE_INTERNAL)
		{
			/* add (beta,beta) matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, beta_gID, beta_gID, beta_lID, beta_lID);CHKERRQ(ierr);

			/* add (omega,beta) and (beta,omega) mixed matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, omeg_gID, beta_gID, omeg_lID, beta_lID);CHKERRQ(ierr);
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, beta_gID, omeg_gID, beta_lID, omeg_lID);CHKERRQ(ierr);
		}

		if (alph_nType == NODE_INTERNAL &&
			beta_nType == NODE_INTERNAL)
		{
			/* add (alpha,beta) and (beta,alpha) mixed matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, alph_gID, beta_gID, alph_lID, beta_lID);CHKERRQ(ierr);
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, beta_gID, alph_gID, beta_lID, alph_lID);CHKERRQ(ierr);
		}

		/*
		 * add vector contributions
		 */

		if (alph_nType == NODE_BOUNDARY ||
			beta_nType == NODE_BOUNDARY)
		{
			/* add (omega) vector contributions */
			ierr = addVecSingleBendCont(globalVec_B, localVec, N, omeg_gID, omeg_lID);CHKERRQ(ierr);

			if (alph_nType == NODE_BOUNDARY && 
				beta_nType == NODE_INTERNAL)
			{
				/* add (beta) vector contributions */
				ierr = addVecSingleBendCont(globalVec_B, localVec, N, beta_gID, beta_lID);CHKERRQ(ierr);
			}

			if (alph_nType == NODE_INTERNAL &&
				beta_nType == NODE_BOUNDARY)
			{
				/* add (alpha) vector contributions */
				ierr = addVecSingleBendCont(globalVec_B, localVec, N, alph_gID, alph_lID);CHKERRQ(ierr);
			}
		}

	}

	return ierr;
}
