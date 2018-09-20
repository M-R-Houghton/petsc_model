#ifndef SYSTEM_SOLVE_H
#define SYSTEM_SOLVE_H

#include "systemAssembly.h"

/**
 * \brief Initiates system solve routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U);

#endif