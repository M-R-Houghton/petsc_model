#include "globalAssemblyBend.h"


/* Adds a single bending contribution of 9 values to the global matrix */
PetscErrorCode addMatSingleBendCont( Mat globalMat_H, PetscScalar localMat[][9], PetscInt N,
										PetscInt gInd_A, PetscInt gInd_B, PetscInt lInd_A, PetscInt lInd_B )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Adds a single bending contribution of 3 values to the global vector */
PetscErrorCode addVecSingleBendCont( Vec globalVec_B, PetscScalar localVec[], PetscInt N,
										PetscInt gInd_A, PetscInt lInd_A )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Adds local bending contributions to the global matrix and RHS vector */
PetscErrorCode addBendContToGlobal( Mat globalMat_H, Vec globalVec_B, PetscInt N,
									PetscScalar localMat[][9], PetscScalar localVec[],
									Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr )
{
	PetscErrorCode ierr = 0;

	return ierr;
}
