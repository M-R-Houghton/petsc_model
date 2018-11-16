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
        

        VecCreate(PETSC_COMM_WORLD,&glbRhs);
        VecSetFromOptions(glbRhs);
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
    MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,2,2);
    MatSetUp(glbMat);

    MatSetValue(glbMat, 0, 0, 1, INSERT_VALUES);
    MatSetValue(glbMat, 0, 1, 0, INSERT_VALUES);
    MatSetValue(glbMat, 1, 0, 0, INSERT_VALUES);
    MatSetValue(glbMat, 1, 1, 1, INSERT_VALUES);

    MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

    VecSetSizes(glbRhs,PETSC_DECIDE,2);
    VecSet(glbRhs,1.0);
    VecDuplicate(glbRhs,&glbSol);

    EXPECT_EQ(systemSolve(glbMat, glbRhs, glbSol), 0);
}


TEST_F(testSystemSolve, testTripodSolve)
{
    MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,3,3);
    MatSetUp(glbMat);
    MatSetValue(glbMat, 0, 0,   0.0003959445836206, INSERT_VALUES); 
    MatSetValue(glbMat, 0, 1,  -0.0001075029206406, INSERT_VALUES); 
    MatSetValue(glbMat, 0, 2,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 1, 0,  -0.0001075029206406, INSERT_VALUES); 
    MatSetValue(glbMat, 1, 1,   0.0015832388287110, INSERT_VALUES); 
    MatSetValue(glbMat, 1, 2,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 2, 0,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 2, 1,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 2, 2,   0.0001711864792787, INSERT_VALUES); 

    MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

    VecSetSizes(glbRhs,PETSC_DECIDE,3);

    VecSetValue(glbRhs, 0,  5.623448489974873e-06, INSERT_VALUES);
    VecSetValue(glbRhs, 1, -1.123790521709401e-05, INSERT_VALUES);
    VecSetValue(glbRhs, 2,  0.000000000000000e+00, INSERT_VALUES);

    VecDuplicate(glbRhs,&glbSol);

    systemSolve(glbMat, glbRhs, glbSol);

    // Test diagonal matrix values
    PetscScalar *sol;
    VecGetArray(glbSol, &sol);
    EXPECT_FLOAT_EQ(sol[0],  1.2505980415641637e-02);
    EXPECT_FLOAT_EQ(sol[1], -6.2488840076005414e-03);
    EXPECT_FLOAT_EQ(sol[2],  0.0000000000000000e+00);
    VecRestoreArray(glbSol, &sol);
}


struct testSystemTimeStepSolve : ::testing::Test
{
    PetscMPIInt     size;
    Box             *box_ptr;
    Parameters      *par_ptr;
    Mat             glbMat;
    Vec             glbRhs;
    Vec             glbSol;
    PetscInt        N;

    void SetUp()
    {
        MPI_Comm_size(PETSC_COMM_WORLD,&size);
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);

        VecCreate(PETSC_COMM_WORLD,&glbRhs);
        VecSetFromOptions(glbRhs);

        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);

        N = 1;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
        MatDestroy(&glbMat);
        VecDestroy(&glbRhs);
        VecDestroy(&glbSol);
    }
};


TEST_F(testSystemTimeStepSolve, testErrorOutput)
{
    // Set H to the identity matrix
    MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,2,2);
    MatSetUp(glbMat);

    MatSetValue(glbMat, 0, 0, 1, INSERT_VALUES);
    MatSetValue(glbMat, 0, 1, 0, INSERT_VALUES);
    MatSetValue(glbMat, 1, 0, 0, INSERT_VALUES);
    MatSetValue(glbMat, 1, 1, 1, INSERT_VALUES);

    MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

	MatView(glbMat,PETSC_VIEWER_STDOUT_WORLD);
    
    // Reset to identity matrix
    //MatZeroEntries(glbMat);
    //MatShift(glbMat, 1.0);

    // Set up rhs B vector
    VecSetSizes(glbRhs,PETSC_DECIDE,2);
    VecSet(glbRhs,1.0);
    VecDuplicate(glbRhs,&glbSol);

    // Initial guess for U
    VecSet(glbSol,0.0);

    EXPECT_EQ(systemTimeStepSolve(glbMat, glbRhs, glbSol), 0);
}


TEST_F(testSystemTimeStepSolve, testTripodSolve)
{
    MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,3,3);
    MatSetUp(glbMat);
    MatSetValue(glbMat, 0, 0,   0.0003959445836206, INSERT_VALUES); 
    MatSetValue(glbMat, 0, 1,  -0.0001075029206406, INSERT_VALUES); 
    MatSetValue(glbMat, 0, 2,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 1, 0,  -0.0001075029206406, INSERT_VALUES); 
    MatSetValue(glbMat, 1, 1,   0.0015832388287110, INSERT_VALUES); 
    MatSetValue(glbMat, 1, 2,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 2, 0,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 2, 1,   0.0000000000000000, INSERT_VALUES); 
    MatSetValue(glbMat, 2, 2,   0.0001711864792787, INSERT_VALUES); 

    MatAssemblyBegin(glbMat,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(glbMat,MAT_FINAL_ASSEMBLY);

    // Set up rhs B
    VecSetSizes(glbRhs,PETSC_DECIDE,3);
    VecSetValue(glbRhs, 0,  5.623448489974873e-06, INSERT_VALUES);
    VecSetValue(glbRhs, 1, -1.123790521709401e-05, INSERT_VALUES);
    VecSetValue(glbRhs, 2,  0.000000000000000e+00, INSERT_VALUES);

    // Set up initial U
    VecDuplicate(glbRhs,&glbSol);
    VecSet(glbSol,0.0);

    // Time step solve
    systemTimeStepSolve(glbMat, glbRhs, glbSol);

    // Check values against direct solve
    PetscScalar *sol;
    VecGetArray(glbSol, &sol);
    EXPECT_FLOAT_EQ(sol[0],  1.2505980415641637e-02);
    EXPECT_FLOAT_EQ(sol[1], -6.2488840076005414e-03);
    EXPECT_FLOAT_EQ(sol[2],  0.0000000000000000e+00);
    VecRestoreArray(glbSol, &sol);
}


} /* namespace */
