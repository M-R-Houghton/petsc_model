#ifndef LOCAL_ASSEMBLY_BEND_H
#define LOCAL_ASSEMBLY_BEND_H

#include "localAssemblyStretch.h"

/**
 * \brief Calculates the bending term kappa
 * \param radius
 * \param youngsModulus
 * \return
 */
PetscScalar calculateKappa(PetscScalar radius, PetscScalar youngsModulus);

/**
 * \brief Adds local bend information for a single fibre to global system
 * Loops through a fibre in triplets
 * \param globalMat_H The global matrix to add contributions into
 * \param globalVec_B The global RHS vector to add contributions into
 * \param N Internal node count
 * \param fibre_ptr The fibre to loop over
 * \param xyzPer
 * \param xyzDim
 * \param youngsModulus
 * \return
 */
PetscErrorCode addFibreLocalBend(Mat globalMat_H, Vec globalVec_B, PetscInt N, const Fibre *fibre_ptr, 
                                    const PetscInt *xyzPer, const PetscScalar *xyzDim, PetscScalar youngsModulus);

/**
 * \brief Assembles the local 2D bend matrix of a given triplet
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendMat_A
 */
PetscErrorCode make2DBendMat(const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                PetscScalar bConst, PetscScalar localBendMat_A[9][9] );

/**
 * \brief Assembles the local 2D bend RHS vector of a given triplet
 * \param u_alphOmeg
 * \param u_omegBeta
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make2DBendVec( const PetscScalar *u_alphOmeg, const PetscScalar *u_omegBeta,
                                const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                PetscScalar bConst, PetscScalar localBendVec_b[9] );

/**
 * \brief Assembles the local 3D bend matrix of a given triplet
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendMat_A
 */
PetscErrorCode make3DBendMat(const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                PetscScalar bConst, PetscScalar localBendMat_A[9][9] );

/**
 * \brief Assembles the local 3D bend RHS vector of a given triplet
 * \param u_alphOmeg
 * \param u_omegBeta
 * \param u_alphBeta
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make3DBendVec( const PetscScalar *u_alphOmeg, const PetscScalar *u_omegBeta, const PetscScalar *u_alphBeta, 
                                const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                PetscScalar bConst, PetscScalar localBendVec_b[9] );


#endif
