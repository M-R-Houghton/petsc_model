#ifndef MIN_UNIT_MACROS_H
#define MIN_UNIT_MACROS_H

#include <stdio.h>
#include <petscksp.h>
#include "debugMacros.h"

/* file: minunit.h */
#define mu_assert(test, message) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char *message = test(); tests_run++; \
                            if (message) return message; } while (0)
extern int tests_run;

const char *test_foo();

const char *test_bar();

const char *all_tests();

#endif