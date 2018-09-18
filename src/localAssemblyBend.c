#include "localAssemblyBend.h"

/* Checks for legal kappa calculation arguments */
void checkKappaArguments(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	assert(fIndex >= 0);
}


/* Calculates the bending term kappa */
PetscScalar calculateKappa(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	/* validate arguments */
	checkKappaArguments(box_ptr, par_ptr, fIndex);

	PetscScalar radius 	= box_ptr->masterFibreList[fIndex].radius;
    PetscScalar yMod 	= par_ptr->youngsModulus;
    PetscScalar area 	= M_PI * pow(radius, 2);

    return (area * pow(radius, 2) * yMod) / 4.0;
}


/* Adds local bend information for a single fibre to global system */
PetscErrorCode addFibreLocalBend(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex)
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 2D bend matrix of a given triplet */
PetscErrorCode make2DBendMat(PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr, 
								PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 2D bend RHS vector of a given triplet */
PetscErrorCode make2DBendVec(Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr,
                    			PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr,
                    			PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 3D bend matrix of a given triplet */
PetscErrorCode make3DBendMat(PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr, 
								PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 3D bend RHS vector of a given triplet */
PetscErrorCode make3DBendVec(Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr,
                    			PetscScalar *s_alphOmeg_ptr, PetscScalar *s_omegBeta_ptr,
                    			PetscScalar *s_alphBeta_ptr, PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}
