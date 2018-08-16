#ifndef NETWORK_BUILD_TESTS_H
#define NETWORK_BUILD_TESTS_H

#include "networkBuild.h"

/**
 * \brief Tests network build routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_networkBuild();

/**
 * \brief Tests parameter builder.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_makeParameters();

/**
 * \brief Tests sparse matrix builder.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_makeSparse();

/**
 * \brief Tests box argument checker.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_checkBoxArguments();

/**
 * \brief Tests box builder.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_makeBox();

/**
 * \brief Tests fibre builder.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_makeFibre();

/**
 * \brief Tests node builder.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_makeNode();

#endif