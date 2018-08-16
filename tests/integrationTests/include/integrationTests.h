#ifndef INTEGRATION_TESTS_H
#define INTEGRATION_TESTS_H

#include "networkBuildTests.h"
#include "networkReadTests.h"
#include "systemAssemblyTests.h"
#include "systemSolveTests.h"
#include "networkAnalysisTests.h"
#include "networkWriteTests.h"
#include "networkDestroyTests.h"

/**
 * \brief Initiates unit test routine
 * \return Index to represent Petsc error code.
 */
PetscErrorCode runIntegrationTests();

#endif