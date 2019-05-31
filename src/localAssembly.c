#include "localAssembly.h"

/* Adds local contributions for every fibre to global system */
PetscErrorCode addLocalContributions(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B)
{
    PetscErrorCode 	ierr;
    PetscInt 		fIndex;

    /* assemble local information by looping over all fibres */
    for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
    {
        /* assemble stretching contributions of every fibre */
        ierr = addFibreLocalStretch(box_ptr, par_ptr, globalMat_H, globalVec_B, fIndex);CHKERRQ(ierr);

        /* only assemble local bending information if macro permits it */
        if (SPAN == 2)
        {
            /* assemble bending contributions of every fibre */
            ierr = addFibreLocalBend(box_ptr, par_ptr, globalMat_H, globalVec_B, fIndex);CHKERRQ(ierr);
        }
    }

    return ierr;
}
