#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "localAssembly.h"
}


namespace {

struct testAddLocalContributions : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat testMatrix;
    Vec testVector;
    PetscInt size;

    void SetUp()
    {
        size = 3;

        MatCreate(PETSC_COMM_WORLD,&testMatrix);
        MatSetFromOptions(testMatrix);
        MatSetSizes(testMatrix,PETSC_DECIDE,PETSC_DECIDE,size,size);
        MatSetUp(testMatrix);

        VecCreate(PETSC_COMM_WORLD,&testVector);
        VecSetFromOptions(testVector);
        VecSetSizes(testVector,PETSC_DECIDE,size);

        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 0.05, 1.0);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
        MatDestroy(&testMatrix);
        VecDestroy(&testVector);
    }
};


TEST_F(testAddLocalContributions, testErrorOutput) 
{
    EXPECT_EQ(addLocalContributions(box_ptr, par_ptr, testMatrix, testVector), 0);
}


TEST_F(testAddLocalContributions, testMatrixValues) 
{
    addLocalContributions(box_ptr, par_ptr, testMatrix, testVector);

    MatAssemblyBegin(testMatrix,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(testMatrix,MAT_FINAL_ASSEMBLY);

    Vec diags;
    PetscScalar *diagonal;
    VecDuplicate(testVector,&diags);
    MatGetDiagonal(testMatrix,diags);

    VecGetArray(diags, &diagonal);
    EXPECT_NEAR(diagonal[0], 0.0003959445836206, 1e-16);
    EXPECT_NEAR(diagonal[1], 0.0015832388287110, 1e-16);
    EXPECT_NEAR(diagonal[2], 0.0001711864792787, 1e-16);
    VecRestoreArray(diags, &diagonal);
    /*
    MPI_Comm comm = PETSC_COMM_WORLD;
    PetscViewer viewer;
    PetscViewerASCIIOpen(comm, "Amat.m", &viewer);
    PetscViewerPushFormat(viewer, PETSC_VIEWER_ASCII_DENSE);
    MatView(testMatrix,viewer);
    PetscViewerPopFormat(viewer);
    PetscViewerDestroy(&viewer);
    */
}


TEST_F(testAddLocalContributions, testVectorValues) 
{
    addLocalContributions(box_ptr, par_ptr, testMatrix, testVector);

    PetscScalar *array;

    VecGetArray(testVector, &array);
    EXPECT_NEAR(array[0],  0.0000056234484900, 1e-16);
    EXPECT_NEAR(array[1], -0.0000112379052171, 1e-16);
    EXPECT_NEAR(array[2],  0.0000000000000000, 1e-16);
    VecRestoreArray(testVector, &array);
    /*
    MPI_Comm comm = PETSC_COMM_WORLD;
    PetscViewer viewer;
    PetscViewerASCIIOpen(comm, "Amat.m", &viewer);
    PetscViewerPushFormat(viewer, PETSC_VIEWER_ASCII_DENSE);
    MatView(testMatrix,viewer);
    PetscViewerPopFormat(viewer);
    PetscViewerDestroy(&viewer);
    */
}

} /* namespace */