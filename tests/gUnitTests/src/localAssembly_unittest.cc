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

        par_ptr = makeParameters(0.05, 1.0);
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
    EXPECT_DOUBLE_EQ(diagonal[0], 0.0003958007154149);
    EXPECT_DOUBLE_EQ(diagonal[1], 0.0015832028616595);
    EXPECT_DOUBLE_EQ(diagonal[2], 0.0001710066440216);
    VecRestoreArray(diags, &diagonal);
    /*
    MPI_Comm comm = PETSC_COMM_WORLD;
    PetscViewer viewer;
    PetscViewerASCIIOpen(comm, "Amat.m", &viewer);
    PetscViewerPushFormat(viewer, PETSC_VIEWER_ASCII_DENSE);
    MatView(testMatrix,viewer);
    PetscViewerPopFormat(viewer);
    PetscViewerDestroy(&viewer);*/
}

} /* namespace */