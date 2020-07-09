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
PetscScalar calculateSegStretchEnergy( const Fibre *fibre_ptr, PetscScalar youngsModulus,
                                        const PetscScalar *s_alph, const PetscScalar *s_beta,
                                        const PetscScalar *u_alph, const PetscScalar *u_beta,
                                        const PetscInt *xyzPer, const PetscScalar *xyzDim );

/**
 * \brief Calculates the bend energy of a given fibre segment
 * \param box_ptr
 * \param par_ptr
 * \param fIndex
 * \return
 */
PetscScalar calculateSegBendEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
                                     const PetscScalar *s_alph, const PetscScalar *s_omeg, const PetscScalar *s_beta,
                                     const PetscScalar *u_alph, const PetscScalar *u_omeg, const PetscScalar *u_beta );

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
 * \brief Adds the relevant sheet based energies of a specific fibre index to the totals.
 * \param box_ptr Box containing sheet network stats.
 * \param par_ptr Parameters containing sheet network energies.
 * \param fIndex Fibre index
 * \return Petsc error code.
 */
PetscErrorCode addFibreSheetEnergy(const Box *box_ptr, Parameters *par_ptr, PetscInt fIndex);

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
 * \brief Prints whether a segment is stretched/compressed 
 * NOTE: Currently just used for debugging some simple lambda cases, but may be expanded.
 * \param s_ab The distance vector.
 * \param u_ab The distance displacement vector.
 * \return Index to represent Petsc error code.
 */
PetscErrorCode printStretchOrCompression(PetscScalar *s_ab, PetscScalar *u_ab);

/**
 * \brief Calculates stretching/compression stats for the base lmb case
 * NOTE: Currently just used for debugging some simple lambda cases, but may be expanded.
 * \param box_ptr
 * \param par_ptr
 * \return Index to represent Petsc error code.
 */
PetscErrorCode stretchCompressionBaseLmb(Box *box_ptr, Parameters *par_ptr);

/**
 * \brief Calculates stretching/compression stats for the off plane lmb variant
 * NOTE: Currently just used for debugging some simple lambda cases, but may be expanded.
 * \param box_ptr
 * \param par_ptr
 * \return Index to represent Petsc error code.
 */
PetscErrorCode stretchCompressionOfpVarLmb(Box *box_ptr, Parameters *par_ptr);

#endif
