#ifndef NETWORK_DESTROY_TESTS_H
#define NETWORK_DESTROY_TESTS_H

#include "networkDestroy.h"

/**
 * \brief Tests network destroy routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_networkDestroy();

/**
 * \brief Tests parameter destroyer.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_destroyParameters();

/**
 * \brief Tests sparse matrix destroyer.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_destroySparse();

/**
 * \brief Tests box destroyer.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode test_destroyBox();

#endif