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