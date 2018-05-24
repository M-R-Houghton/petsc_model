/* file minunit_example.c */

#include "unitTests.h"

int tests_run = 0;
 
const char *test_foo() 
{
    int foo = 7;
    mu_assert(foo == 7, "error, foo != 7");
    return 0;
}

const char *test_bar() 
{
    int bar = 5;
    mu_assert(bar == 5, "error, bar != 5");
    return 0;
}

const char *new_test()
{
    mu_assert(2 == 2, "error here");
    return 0;
}

const char *all_tests() 
{
    mu_run_test(test_foo);
    mu_run_test(test_bar);
    mu_run_test(new_test);
    return 0;
}
 
int runUnitTests(PetscErrorCode ierr) 
{
    // run network read in tests
    ierr = test_networkRead();

    ierr = test_systemAssembly();

    ierr = test_systemSolve();

    ierr = test_networkAnalysis();

    ierr = test_networkWrite();

    //*
    const char *result = all_tests();
    if (result != 0) 
    {
        log_info("%s\n", result);
    }
    else 
    {
        log_info("ALL TESTS PASSED");
    }
    log_info("Tests run: %d", tests_run);
    //*/

    return ierr;
}
