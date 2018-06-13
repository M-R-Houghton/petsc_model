#ifndef NETWORK_DESTROY_H
#define NETWORK_DESTROY_H

#include "networkBuild.h"

/**
 * \brief Initiates network destroy routine.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkDestroy();

/**
 * \brief Destroys a parameters structure
 * \param par_ptr
 */
void destroyParameters(Parameters *par_ptr);

/**
 * \brief Destroys a sparse structure
 * \param sparse_ptr
 */
void destroySparse(Sparse *sparse_ptr);

/**
 * \brief Destroys a box structure
 * \param box_ptr
 */
void destroyBox(Box *box_ptr);

#endif