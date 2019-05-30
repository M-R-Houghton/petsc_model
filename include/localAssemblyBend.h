#ifndef LOCAL_ASSEMBLY_BEND_H
#define LOCAL_ASSEMBLY_BEND_H

#include "localAssemblyStretch.h"

/**
 * \brief Checks for legal kappa calculation arguments
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 */
void checkKappaArguments(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Calculates the bending term kappa
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateKappa(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Adds local bend information for a single fibre to global system
 * Loops through a fibre in triplets
 * \param box_ptr
 * \param par_ptr
 * \param globalMat_H
 * \param globalVec_B
 * \param fIndex
 * \return
 */
PetscErrorCode addFibreLocalBend(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex);

/**
 * \brief Assembles the local 2D bend matrix of a given triplet
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendMat_A
 */
PetscErrorCode make2DBendMat(PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta, 
        PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendMat_A[9][9] );

/**
 * \brief Assembles the local 2D bend RHS vector of a given triplet
 * \param u_alphOmeg
 * \param u_omegBeta
 * \param u_alphBeta
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make2DBendVec( const PetscScalar *u_alphOmeg, const PetscScalar *u_omegBeta, const PetscScalar *u_alphBeta,
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
PetscErrorCode make3DBendMat(PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta, 
        PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendMat_A[9][9] );

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


/**
 * \brief Shifts the diagonal of the 3D local bend matrix by a small value lambda
 * \param localBendMat_A
 * \param lambda
 */
PetscErrorCode applyMediumTo3DBendMat(PetscScalar localBendMat_A[9][9], const PetscScalar lambda);

/**
 * \brief Shifts the 3D local stretch vector affinely
 * \param localBendVec_b
 * \param lambda
 * \param u_aff_alph 
 * \param u_aff_omeg 
 * \param u_aff_beta 
 */
PetscErrorCode applyMediumTo3DBendVec(PetscScalar localBendVec_b[9], const PetscScalar lambda, 
                                        const PetscScalar *u_aff_alph, const PetscScalar *u_aff_omeg, const PetscScalar *u_aff_beta);

#endif
