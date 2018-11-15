#ifndef SYSTEM_SOLVE_H
#define SYSTEM_SOLVE_H

#include "systemAssembly.h"

/**
 * \brief Initiates system solve routine.
 * \param globalMat_H The global system matrix H to be solved
 * \param globalVec_B The global system RHS vector
 * \param globalVec_U The global system solution vector
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U);

/**
 * \brief Initiates time-stepping solve routine. 
 * \param globalMat_H The global system matrix H to be solved
 * \param globalVec_B The global system RHS vector
 * \param globalVec_U The global system solution vector
 * \return Index to represent Petsc error code.
 */
PetscErrorCode systemTimeStepSolve(Mat globalMat_H, Vec globalVec_B, Vec globalVec_U);

#endif
