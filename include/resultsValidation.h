#ifndef RESULTS_VALIDATION_H
#define RESULTS_VALIDATION_H

#include "networkWrite.h"

/**
 * \brief Initiates system solve routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U);

#endif