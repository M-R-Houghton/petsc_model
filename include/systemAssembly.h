#ifndef SYSTEM_ASSEMBLY_H
#define SYSTEM_ASSEMBLY_H

#include "networkRead.h"

/**
 * \brief Initiates system assembly routine.
 * \param H Global matrix to be assembled
 * \return Index to represent success or failure.
 */
PetscErrorCode systemAssembly(Mat H);

#endif