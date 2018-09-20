#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "systemSolve.h"
}


namespace {

struct testSystemSolve : ::testing::Test
{
    Mat glbMat;
    Vec glbRhs;
    Vec glbSol;

    void SetUp()
    {
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,2,2);
        MatSetUp(glbMat);

        MatSetValue(glbMat, 0, 0, 1, INSERT_VALUES);
        MatSetValue(glbMat, 0, 1, 0, INSERT_VALUES);
        MatSetValue(glbMat, 1, 0, 0, INSERT_VALUES);
        MatSetValue(glbMat, 1, 1, 1, INSERT_VALUES);

        MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
        MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

        VecCreate(PETSC_COMM_WORLD,&glbRhs);
        VecSetFromOptions(glbRhs);
        VecSetSizes(glbRhs,PETSC_DECIDE,2);

        VecSet(glbRhs,1.0);
        VecDuplicate(glbRhs,&glbSol);
    }

    void TearDown()
    {
        /* clean up */
        VecDestroy(&glbRhs);
        VecDestroy(&glbSol);
        MatDestroy(&glbMat);
    }
};


TEST_F(testSystemSolve, testErrorOutput)
{
    EXPECT_EQ(systemSolve(glbMat, glbRhs, glbSol), 0);
}

} /* namespace */
