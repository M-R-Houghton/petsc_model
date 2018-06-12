#ifndef NETWORK_READ_TESTS_H
#define NETWORK_READ_TESTS_H

#include "networkRead.h"

const char *test_dataRead();
const char *all_data();

/**
 * \brief Tests network read in routine.
 * \return Index to represent success or failure.
 */
PetscErrorCode test_networkRead();

#endif