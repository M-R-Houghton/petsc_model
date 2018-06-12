#ifndef NETWORK_BUILD_H
#define NETWORK_BUILD_H

#include "common.h"

/**
 * \brief Initiates network build routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkBuild();

/**
 * \brief A function for parameters creation
 * \param gamma
 * \param yMod
 * \return
 */
Parameters *makeParameters(PetscScalar gamma, PetscScalar yMod);

#endif