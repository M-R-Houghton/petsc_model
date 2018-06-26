#include "localAssemblyStretch.h"

/* Checks for legal fibre index and segment length */
void checkKArguments(Box *box_ptr, PetscInt fIndex, PetscScalar segLength)
{
	assert(fIndex >= 0);
	assert(segLength >= 0);
	assert(segLength < box_ptr->xyzDimension[0]*0.5);
	assert(segLength < box_ptr->xyzDimension[1]*0.5);
	assert(segLength < box_ptr->xyzDimension[2]*0.5);
}


/* Calculates the stretching constant k */
PetscScalar calculateK(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex, PetscScalar segLength)
{
	/* validate arguments */
	checkKArguments(box_ptr, fIndex, segLength);

	PetscScalar radius 	= box_ptr->masterFibreList[fIndex].radius;
    PetscScalar yMod 	= par_ptr->youngsModulus;
    PetscScalar area 	= M_PI * pow(radius, 2);
    PetscScalar mu 		= area * yMod;

    return mu / segLength;
}