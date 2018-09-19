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
 * \param u_alph
 * \param u_omeg
 * \param u_beta
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make2DBendVec(PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta,
                    			PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta,
                    			PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendVec_b[9] );

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
 * \param u_alph
 * \param u_omeg
 * \param u_beta
 * \param s_alphOmeg
 * \param s_omegBeta
 * \param s_alphBeta
 * \param bConst
 * \param localBendVec_b
 */
PetscErrorCode make3DBendVec(PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta,
                    			PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta,
                    			PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendVec_b[9] );




#endif
