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
 * \brief Checks for nans and infinite values.
 * May replace this with something more graceful if the output file is needed in future.
 * \param displacement The solved displacement that needs to be verified.
 */
void checkForValidDisplacement(PetscScalar displacement);

/**
 * \brief Updates a single internal node with its relevant solved displacement 
 * \param node_ptr Pointer to the node that is to be updated.
 * \param N The internal node count.
 * \param globalVec_U Solution vector to collect update data from.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode updateInternalNodeDisp(Node *node_ptr, PetscInt N, Vec globalVec_U);

/**
 * \brief Checks for dangling fibres that should be avoided during network generation 
 * \param fibre_ptr Pointer to the fibre to be checked.
 * \param nOnFibre Number of nodes on each fibre
 * \return Index to represent Petsc error code.
 */
PetscBool isDanglingFibre(const Fibre *fibre_ptr, PetscInt nOnFibre);

/* 
 * \brief Updates the displacements of dangling nodes of dangling fibres 
 * \param bgn The first node in the triple node dangling fibre
 * \param mid The middle (non-dangling) node in the triple node dangling fibre
 * \param end The last node in the triple node dangling fibre
 */
PetscErrorCode updateDanglingFibreNodeDisp(Node *bgn, const Node *mid, Node *end);

/**
 * \brief Updates a single dangling node using its 2 closest neighbouring nodes
 * \param box_ptr Pointer to box, needed for making relevant distance vectors.
 * \param alph_ptr Pointer to the node 2nd closest to the dangling node.
 * \param beta_ptr Pointer to the node closest to the dangling node.
 * \param delt_ptr Pointer to the dangling node that is to be updated.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode updateDanglingNodeDisp(Box *box_ptr, const Node *alph_ptr, const Node *beta_ptr, Node *delt_ptr);

#endif
