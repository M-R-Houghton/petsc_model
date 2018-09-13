#ifndef LOCAL_ASSEMBLY_H
#define LOCAL_ASSEMBLY_H

#include "localAssemblyBend.h"

/**
 * \brief Adds local contributions for every fibre to global system
 * Loops through every fibre of the network
 * \param box_ptr
 * \param par_ptr
 * \param globalMat_H
 * \param globalVec_B
 * \return
 */
PetscErrorCode addLocalContributions(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B);


#endif