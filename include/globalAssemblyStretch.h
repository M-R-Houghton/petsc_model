#ifndef GLOBAL_ASSEMBLY_STRETCH_H
#define GLOBAL_ASSEMBLY_STRETCH_H

#include "networkRead.h"

/**
 * \brief Adds a single stretching contribution of 9 values to the global matrix
 * Extracts 9 values from a local stretch matrix and adds them to the global using global indexes
 * \param globalMat_H The global matrix H.
 * \param alpha_gInd The global index for node alpha.
 * \param beta_gInd The global index for node beta.
 * \param alpha_lInd The local index for node alpha.
 * \param beta_lInd The local index for node beta.
 * \return Petsc error code.
 */
PetscErrorCode addSingleStretchCont(Mat globalMat_H, PetscInt alpha_gInd, PetscInt beta_gInd, 
									PetscInt alpha_lInd, PetscInt beta_lInd);


#endif