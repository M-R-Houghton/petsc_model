#include "localAssembly.h"

/* Adds local contributions for every fibre to global system */
PetscErrorCode addLocalContributions(const Box *box_ptr, const Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B)
{
    PetscErrorCode  ierr;
    PetscInt        fIndex;

    /* assemble local information by looping over all fibres */
    for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
    {
        const Fibre *fibre_ptr = &box_ptr->masterFibreList[fIndex];

        /* assemble stretching contributions of every fibre */
        ierr = addFibreLocalStretch(globalMat_H, globalVec_B, box_ptr->nodeInternalCount, fibre_ptr,
                                    box_ptr->xyzPeriodic, box_ptr->xyzDimension, par_ptr->youngsModulus);
        CHKERRQ(ierr);

        /* only assemble local bending information if macro permits it */
        if (SPAN == 2)
        {
            /* assemble bending contributions of every fibre */
            ierr = addFibreLocalBend(globalMat_H, globalVec_B, box_ptr->nodeInternalCount, fibre_ptr,
                                        box_ptr->xyzPeriodic, box_ptr->xyzDimension, par_ptr->youngsModulus);
            CHKERRQ(ierr);
        }
    }

    return ierr;
}
