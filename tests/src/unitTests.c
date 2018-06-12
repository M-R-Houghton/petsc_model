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
 
PetscErrorCode runUnitTests() 
{
    PetscErrorCode ierr;
    ierr = PetscPrintf(PETSC_COMM_WORLD,"===================================\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"\t BEGINNING UNIT TESTS\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"===================================\n");CHKERRQ(ierr);
    
    // run network building tests
    ierr = test_networkBuild();CHKERRQ(ierr);

    // run network read in tests
    ierr = test_networkRead();CHKERRQ(ierr);

    // run assembly tests
    ierr = test_systemAssembly();CHKERRQ(ierr);

    // run solver tests
    ierr = test_systemSolve();CHKERRQ(ierr);

    // run network prediction tests
    ierr = test_networkAnalysis();CHKERRQ(ierr);

    // run network write out tests
    ierr = test_networkWrite();CHKERRQ(ierr);

    /*
    const char *result = all_tests();
    if (result != 0) 
    {
        log_info("%s\n", result);
    }
    else 
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    */

    ierr = PetscPrintf(PETSC_COMM_WORLD,"===================================\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"\t FINISHED UNIT TESTS\n");CHKERRQ(ierr);
    ierr = PetscPrintf(PETSC_COMM_WORLD,"===================================\n");CHKERRQ(ierr);

    return ierr;
}
