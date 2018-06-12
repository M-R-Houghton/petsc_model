#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include "networkBuildTests.h"
#include "networkReadTests.h"
#include "systemAssemblyTests.h"
#include "systemSolveTests.h"
#include "networkAnalysisTests.h"
#include "networkWriteTests.h"

/**
 * \brief Initiates unit test routine
 * \return Index to represent Petsc error code.
 */
PetscErrorCode runUnitTests();

#endif