#ifndef NETWORK_UPDATE_H
#define NETWORK_UPDATE_H

#include "systemSolve.h"

/**
 * \brief Initiates network update routine.
 * \param box_ptr Pointer to box for data that is to be updated.
 * \param globalVec_U Solution vector to collect update data from.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkUpdate(Box *box_ptr, Vec globalVec_U);

/**
 * \brief Updates a single internal node with its relevant solved displacement 
 * \param node_ptr Pointer to the node that is to be updated.
 * \param N The internal node count.
 * \param globalVec_U Solution vector to collect update data from.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode updateInternalNodeDisp(Node *node_ptr, PetscInt N, Vec globalVec_U);

#endif