#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

#include "networkDestroy.h"

/**
 * \brief Calculates the minimum of two integers
 * \param a
 * \param b
 * \return The minimum of a and b
 */
PetscInt minInt(PetscInt a, PetscInt b);

/**
 * \brief Calculates the maximum of two integers
 * \param a
 * \param b
 * \return The maximum of a and b
 */
PetscInt maxInt(PetscInt a, PetscInt b);

/**
 * \brief Calculates the minimum of two scalars
 * \param a
 * \param b
 * \return The minimum of a and b
 */
PetscScalar minScalar(PetscScalar a, PetscScalar b);

/**
 * \brief Calculates the maximum of two scalars
 * \param a
 * \param b
 * \return The maximum of a and b
 */
PetscScalar maxScalar(PetscScalar a, PetscScalar b);

/**
 * \brief Calculates the dot product of two given vectors
 * \param vec1_ptr
 * \param vec2_ptr
 * \return
 */
PetscScalar vecDotProduct(PetscScalar *vec1_ptr, PetscScalar *vec2_ptr);

/**
 * \brief Calculates the cross product of two given 2D vectors
 * \param vec1_ptr
 * \param vec2_ptr
 * \return
 */
PetscScalar vec2DCrossProduct(PetscScalar *vec1_ptr, PetscScalar *vec2_ptr);

/**
 * \brief Calculates the cross product of two given 3D vectors
 * \param crossVec_ptr
 * \param vec1_ptr
 * \param vec2_ptr
 */
PetscErrorCode vec3DCrossProduct(PetscScalar *crossVec_ptr, PetscScalar *vec1_ptr, PetscScalar *vec2_ptr);

/**
 * \brief Calculates the magnitude of a given vector
 * \param vec_ptr
 * \return
 */
PetscScalar vecMagnitude(PetscScalar *vec_ptr);

/**
 * \brief Calculates the addition of two position vectors
 * \param addVec_ptr
 * \param posVec1_ptr
 * \param posVec2_ptr
 * \param box_ptr
 */
PetscErrorCode vecAddition(PetscScalar *addVec_ptr, PetscScalar *posVec1_ptr, PetscScalar *posVec2_ptr, Box *box_ptr);

/**
 * \brief Creates the unit tangent vector of a given vector
 * \param tangVec_ptr
 * \param vec_ptr
 */
PetscErrorCode makeTangentVec(PetscScalar *tangVec_ptr, PetscScalar *vec_ptr);

/**
 * \brief Creates the distance vector between two position vectors
 * \param distVec_ptr
 * \param posVec1_ptr
 * \param posVec2_ptr
 * \param box_ptr
 */
PetscErrorCode makeDistanceVec(PetscScalar *distVec_ptr, PetscScalar *posVec1_ptr, PetscScalar *posVec2_ptr, Box *box_ptr);

/**
 * \brief Creates the position vector of a given node
 * \param posVec_ptr
 * \param node_ptr
 */
PetscErrorCode makePositionVec(PetscScalar *posVec_ptr, Node *node_ptr);

/**
 * \brief Updates a position vector with the displacement of the corresponding node
 * \param posVec_ptr
 * \param node_ptr
 */
PetscErrorCode updatePositionVec(PetscScalar *posVec_ptr, Node *node_ptr);

/**
 * \brief Creates the displacement vector of a given node
 * \param dispVec_ptr
 * \param node_ptr
 */
PetscErrorCode makeDisplacementVec(PetscScalar *dispVec_ptr, Node *node_ptr);

/**
 * \brief Checks whether a segment crosses the N boundary and updates it to the nearest copy inside the domain
 * \param distVec_ptr
 * \param box_ptr
 * \param N The relevant boundary to check for intersection with
 */
PetscErrorCode nearestSegmentCopyDirN(PetscScalar *distVec_ptr, Box *box_ptr, PetscInt N);

/**
 * \brief Checks whether a segment crosses any boundary and updates it to the nearest copy inside the domain
 * \param distVec_ptr
 * \param box_ptr
 */
PetscErrorCode nearestSegmentCopy(PetscScalar *distVec_ptr, Box *box_ptr);


#endif