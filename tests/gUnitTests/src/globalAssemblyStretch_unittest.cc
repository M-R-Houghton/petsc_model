#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "globalAssemblyStretch.h"
}


namespace {

struct testAddMatSingleStretchCont : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat glbMat;
    PetscScalar locMat[6][6];

    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,3,3);
        MatSetUp(glbMat);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        MatDestroy(&glbMat);
    }
};


TEST_F(testAddMatSingleStretchCont, testErrorOutput) 
{
    PetscInt N = box_ptr->nodeInternalCount;
    EXPECT_EQ(addMatSingleStretchCont(glbMat,locMat,N,0,0,0,0), 0);
    //EXPECT_EQ(addMatSingleStretchContFAST(glbMat,locMat,N,0,0,0,0), 0);
}


struct testAddVecSingleStretchCont : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Vec glbVec;
    PetscScalar locVec[6];

    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,3);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        VecDestroy(&glbVec); 
    }
};


TEST_F(testAddVecSingleStretchCont, testErrorOutput) 
{
    PetscInt N = box_ptr->nodeInternalCount;
    EXPECT_EQ(addVecSingleStretchCont(glbVec,locVec,N,0,0), 0);
}


struct testAddStretchContToGlobal : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat glbMat;
    Vec glbVec;
    PetscScalar locMat[6][6], locVec[6];
    Node *alpha_ptr, *beta_ptr;
    PetscInt N;

    void SetUp()
    {
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,6,6);
        MatSetUp(glbMat);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,6);

        locMat[0][0] = 0;
        locVec[0] = 0;

        const char fileToRead[] = "data/exAssembleSystem.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alpha_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr = &(box_ptr->masterNodeList[1]);

        N = 2;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        MatDestroy(&glbMat);
        VecDestroy(&glbVec);
    }
};


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphInternalBetaInternal)
{
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphInternalBetaDangling)
{
    beta_ptr->nodeType = 1, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphInternalBetaBoundary)
{
    beta_ptr->nodeType = 2, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphDanglingBetaInternal)
{
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphDanglingBetaDangling)
{
    beta_ptr->nodeType = 1, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphDanglingBetaBoundary)
{
    beta_ptr->nodeType = 2, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphBoundarylBetaInternal)
{
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphBoundarylBetaDangling)
{
    beta_ptr->nodeType = 1, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}


TEST_F(testAddStretchContToGlobal, testErrorOutputAlphBoundarylBetaBoundary)
{
    beta_ptr->nodeType = 2, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,N,locMat,locVec), 0);
}

} /* namespace */
