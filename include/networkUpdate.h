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

/**
 * \brief Updates a single dangling node using its 2 closest neighbouring nodes
 * \param box_ptr Pointer to box, needed for making relevant distance vectors.
 * \param alph_ptr Pointer to the node 2nd closest to the dangling node.
 * \param beta_ptr Pointer to the node closest to the dangling node.
 * \param delt_ptr Pointer to the dangling node that is to be updated.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode updateDanglingNodeDisp(Box *box_ptr, Node *alph_ptr, Node *beta_ptr, Node *delt_ptr);

#endif