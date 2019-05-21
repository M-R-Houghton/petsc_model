#include "globalAssemblyBend.h"


/* Checks bend matrix contribution indexes are all legal */
void checkMatBendContIndexes( const PetscInt gInd_A, const PetscInt gInd_B, 
                                const PetscInt lInd_A, const PetscInt lInd_B )
{
    /* protect from unassigned negative global indexing */
	assert(gInd_A >= 0 && gInd_B >= 0); 
	/* protect from indexing out of range */
	assert(lInd_A == 0 || lInd_A == 1 || lInd_A == 2);
	assert(lInd_B == 0 || lInd_B == 1 || lInd_B == 2);
}


/* Adds a single bending contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleBendContFAST( Mat globalMat_H, const PetscScalar localMat[][9], const PetscInt N,
										 const PetscInt gInd_A, const PetscInt gInd_B, 
                                         const PetscInt lInd_A, const PetscInt lInd_B )
{
    // TODO: Understand why this function doesn't work as expected
	PetscErrorCode 	ierr = 0;
	PetscInt 		row;
	PetscInt 		col[DIMENSION];
	PetscScalar 	val[DIMENSION];
	PetscInt 		i,j;

	checkMatBendContIndexes(gInd_A, gInd_B, lInd_A, lInd_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			row    = gInd_A + i*N;
			col[j] = gInd_B + j*N;
			val[j] = localMat[lInd_A + 3*i][lInd_B + 3*j];
		}
		ierr = MatSetValues(globalMat_H, 1, &row, 3, col, val, ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds a single bending contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleBendCont( Mat globalMat_H, const PetscScalar localMat[][9], const PetscInt N,
									 const PetscInt gInd_A, const PetscInt gInd_B, 
                                     const PetscInt lInd_A, const PetscInt lInd_B )
{
	PetscErrorCode 	ierr = 0;
	PetscInt 		i,j;

	checkMatBendContIndexes(gInd_A, gInd_B, lInd_A, lInd_B);

	for (i = 0; i < DIMENSION; i++)
	{
		for (j = 0; j < DIMENSION; j++)
		{
			ierr = MatSetValue(globalMat_H, gInd_A + i*N, gInd_B + j*N, localMat[lInd_A + 3*i][lInd_B + 3*j], ADD_VALUES);CHKERRQ(ierr);
			/* WARNING: For debugging ONLY */
			//ierr = PetscPrintf(PETSC_COMM_WORLD,"[BCONT] %0.16g\t", localMat[lInd_A + 3*i][lInd_B + 3*j]);CHKERRQ(ierr);
		}
		//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);
	}
	//ierr = PetscPrintf(PETSC_COMM_WORLD, "\n");CHKERRQ(ierr);

	return ierr;
}


/* Checks bend vector contribution indexes are all legal */
void checkVecBendContIndexes( const PetscInt gInd_A, const PetscInt lInd_A )
{
	assert(gInd_A >= 0);
	assert(lInd_A == 0 || lInd_A == 1 || lInd_A == 2);
}


/* Adds a single bending contribution of 3 values to the global vector */
PetscErrorCode addVecSingleBendCont( Vec globalVec_B, const PetscScalar localVec[], 
                                     const PetscInt N, const PetscInt gInd_A, const PetscInt lInd_A )
{
	PetscErrorCode ierr = 0;
	PetscInt 		i;

	checkVecBendContIndexes(gInd_A, lInd_A);

	for (i = 0; i < DIMENSION; i++)
	{
		ierr = VecSetValue(globalVec_B, gInd_A + i*N, localVec[lInd_A + 3*i], ADD_VALUES);CHKERRQ(ierr);
	}

	return ierr;
}


/* Adds local bending contributions to the global matrix and RHS vector */
PetscErrorCode addBendContToGlobal( Mat globalMat_H, Vec globalVec_B, const PetscInt N,
									const PetscScalar localMat[][9], const PetscScalar localVec[],
									const Node *alph_ptr, const Node *omeg_ptr, const Node *beta_ptr )
{
	PetscErrorCode ierr = 0;

	const PetscInt alph_gID = alph_ptr->globalID;		/* setup global IDs */
	const PetscInt omeg_gID = omeg_ptr->globalID;
	const PetscInt beta_gID = beta_ptr->globalID;

	const PetscInt alph_lID = 0;						/* setup local IDs */
	const PetscInt omeg_lID = 1; 
	const PetscInt beta_lID = 2;

	if (omeg_ptr->nodeType == NODE_INTERNAL &&
		alph_ptr->nodeType != NODE_DANGLING &&
		beta_ptr->nodeType != NODE_DANGLING)
	{
		/*
		 * add matrix contributions
		 */

		//ierr = PetscPrintf(PETSC_COMM_WORLD,"IDs: %d, %d, %d\n", alph_ptr->nodeID, omeg_ptr->nodeID, beta_ptr->nodeID);CHKERRQ(ierr);

		/* add (omega,omega) matrix contributions */
		ierr = addMatSingleBendCont(globalMat_H, localMat, N, omeg_gID, omeg_gID, omeg_lID, omeg_lID);CHKERRQ(ierr);

		if (alph_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (alpha,alpha) matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, alph_gID, alph_gID, alph_lID, alph_lID);CHKERRQ(ierr);

			/* add (alpha,omega) and (omega,alpha) mixed matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, alph_gID, omeg_gID, alph_lID, omeg_lID);CHKERRQ(ierr);
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, omeg_gID, alph_gID, omeg_lID, alph_lID);CHKERRQ(ierr);
		}

		if (beta_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (beta,beta) matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, beta_gID, beta_gID, beta_lID, beta_lID);CHKERRQ(ierr);

			/* add (omega,beta) and (beta,omega) mixed matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, omeg_gID, beta_gID, omeg_lID, beta_lID);CHKERRQ(ierr);
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, beta_gID, omeg_gID, beta_lID, omeg_lID);CHKERRQ(ierr);
		}

		if (alph_ptr->nodeType == NODE_INTERNAL &&
			beta_ptr->nodeType == NODE_INTERNAL)
		{
			/* add (alpha,beta) and (beta,alpha) mixed matrix contributions */
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, alph_gID, beta_gID, alph_lID, beta_lID);CHKERRQ(ierr);
			ierr = addMatSingleBendCont(globalMat_H, localMat, N, beta_gID, alph_gID, beta_lID, alph_lID);CHKERRQ(ierr);
		}

		/*
		 * add vector contributions
		 */

		if (alph_ptr->nodeType == NODE_BOUNDARY ||
			beta_ptr->nodeType == NODE_BOUNDARY)
		{
			/* add (omega) vector contributions */
			ierr = addVecSingleBendCont(globalVec_B, localVec, N, omeg_gID, omeg_lID);CHKERRQ(ierr);

			if (alph_ptr->nodeType == NODE_BOUNDARY && 
				beta_ptr->nodeType == NODE_INTERNAL)
			{
				/* add (beta) vector contributions */
				ierr = addVecSingleBendCont(globalVec_B, localVec, N, beta_gID, beta_lID);CHKERRQ(ierr);
			}

			if (alph_ptr->nodeType == NODE_INTERNAL &&
				beta_ptr->nodeType == NODE_BOUNDARY)
			{
				/* add (alpha) vector contributions */
				ierr = addVecSingleBendCont(globalVec_B, localVec, N, alph_gID, alph_lID);CHKERRQ(ierr);
			}
		}

	}

	return ierr;
}
