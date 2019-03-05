#ifndef NETWORK_ANALYSIS_H
#define NETWORK_ANALYSIS_H

#include "networkUpdate.h"

/**
 * \brief Initiates network analysis routine.
 * \param box_ptr
 * \param par_ptr
 * \return Index to represent Petsc error code.
 */
PetscErrorCode networkAnalysis(Box *box_ptr, Parameters *par_ptr);

/**
 * \brief Calculates the stretch energy of a given fibre segment
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateSegStretchEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
										PetscScalar *s_alph, PetscScalar *s_beta,
                                        PetscScalar *u_alph, PetscScalar *u_beta );

/**
 * \brief Calculates the bend energy of a given fibre segment
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateSegBendEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
									 PetscScalar *s_alph, PetscScalar *s_omeg, PetscScalar *s_beta,
                                     PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta );

/**
 * \brief Calculates the stretch energy of a given fibre
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscErrorCode calculateFibreStretchEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Calculates the bend energy of a given fibre
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscErrorCode calculateFibreBendEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

/**
 * \brief Calculates the energy of an entire network
 * \param box_ptr
 * \param par_ptr
 * \return
 */
PetscErrorCode calculateEnergy(Box *box_ptr, Parameters *par_ptr);

/**
 * \brief Checks for a valid volume
 * \param box_ptr Pointer to box containing dimensions.
 * \param volume Volume to be checked.
 * \return
 */
void checkVolume(Box *box_ptr, PetscScalar volume);

/**
 * \brief Calculates the volume of a network
 * \param box_ptr
 * \return
 */
PetscScalar calculateVolume(Box *box_ptr);

/**
 * \brief Calculates the aperiodic range of a network
 * \param box_ptr
 * \param i, the index to distinguish x,y or z
 * \return
 */
PetscScalar calculateAperiodicRange(Box *box_ptr, PetscInt dim);

/**
 * \brief Calculates the shear modulus of a network
 * \param box_ptr
 * \param par_ptr
 * \return
 */
PetscErrorCode calculateShearModulus(Box *box_ptr, Parameters *par_ptr);

/**
 * \brief Prints out stretching/compression stats
 * NOTE: Currently just used for debugging some simple lambda cases, but may be expanded.
 * \param box_ptr
 * \param par_ptr
 * \return Index to represent Petsc error code.
 */
PetscErrorCode stretchCompressionAnalysis(Box *box_ptr, Parameters *par_ptr);

#endif
