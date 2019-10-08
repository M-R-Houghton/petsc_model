#ifndef LOCAL_ASSEMBLY_STRETCH_H
#define LOCAL_ASSEMBLY_STRETCH_H

#include "globalAssemblyBend.h"

/**
 * \brief Validates that a given segment length is permitted
 * \param segLength
 * \param xyzPer
 * \param xyzDim
 */
void checkSegLength(PetscScalar segLength, const PetscInt *xyzPer, const PetscScalar *xyzDim);

/**
 * \brief Calculates the stretching constant k
 * \param radius
 * \param youngsModulus
 * \param segLength
 * \return
 */
PetscScalar calculateK(PetscScalar radius, PetscScalar youngsModulus, PetscScalar segLength);

/**
 * \brief Calculates information about the current fibre segment pair 
 * \param k
 * \param t_alphBeta
 * \param fibre_ptr
 * \param s_alph
 * \param s_beta
 * \param xyzPer
 * \param xyzDim
 * \param youngsModulus
 * \return Petsc error code
 */
PetscErrorCode calculateSegPairInfo( PetscScalar *k, PetscScalar *t_alphBeta, const Fibre *fibre_ptr, 
                                        const PetscScalar *s_alph, const PetscScalar *s_beta,
                                        const PetscInt *xyzPer, const PetscScalar *xyzDim, PetscScalar youngsModulus);

/**
 * \brief Adds local stretch information for a single fibre to global system
 * Loops through a fibre in pairs
 * \param globalMat_H The global matrix to add contributions into
 * \param globalVec_B The global RHS vector to add contributions into
 * \param N Internal node count
 * \param fibre_ptr The fibre to loop over
 * \param xyzPer
 * \param xyzDim
 * \param youngsModulus
 * \return
 */
PetscErrorCode addFibreLocalStretch(Mat globalMat_H, Vec globalVec_B, PetscInt N, const Fibre *fibre_ptr, 
                                    const PetscInt *xyzPer, const PetscScalar *xyzDim, PetscScalar youngsModulus);

/**
 * \brief Assembles the 2D local stretch matrix of a given pair
 * \param k
 * \param tangVec_ptr
 * \param localStretchMat_A
 */
PetscErrorCode make2DStretchMat(PetscScalar k, const PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6]);

/**
 * \brief Assembles the 2D local stretch RHS vector of a given pair
 * \param u_alphBeta
 * \param k
 * \param tangVec
 * \param localStretchVec_b
 */
PetscErrorCode make2DStretchVec(const PetscScalar *u_alphBeta, PetscScalar k, 
                                const PetscScalar *tangVec, PetscScalar *localStretchVec_b);

/**
 * \brief Assembles the 3D local stretch matrix of a given pair
 * \param k
 * \param tangVec
 * \param localStretchMat_A
 */
PetscErrorCode make3DStretchMat(PetscScalar k, const PetscScalar *tangVec, PetscScalar localStretchMat_A[6][6]);

/**
 * \brief Assembles the 3D local stretch RHS vector of a given pair
 * \param u_alphBeta
 * \param k
 * \param tangVec
 * \param localStretchVec_b
 */
PetscErrorCode make3DStretchVec( const PetscScalar *u_alphBeta, PetscScalar k, 
                                    const PetscScalar *tangVec, PetscScalar *localStretchVec_b );


#endif
