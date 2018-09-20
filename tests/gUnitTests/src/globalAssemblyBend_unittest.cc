#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "globalAssemblyBend.h"
}


namespace {

struct testAddMatSingleBendCont : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat glbMat;
    PetscScalar locMat[9][9];
    PetscInt N;

    void SetUp()
    {
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,3,3);
        MatSetUp(glbMat);

        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        N = box_ptr->nodeInternalCount;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        MatDestroy(&glbMat);
    }
};


TEST_F(testAddMatSingleBendCont, testErrorOutput) 
{
    EXPECT_EQ(addMatSingleBendCont(glbMat,locMat,N,0,0,0,0), 0);
}


struct testAddVecSingleBendCont : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Vec glbVec;
    PetscScalar locVec[9];

    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
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


TEST_F(testAddVecSingleBendCont, testErrorOutput) 
{
    PetscInt N = box_ptr->nodeInternalCount;
    EXPECT_EQ(addVecSingleBendCont(glbVec,locVec,N,0,0), 0);
}


struct testAddBendContToGlobal : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat glbMat;
    Vec glbVec;
    PetscScalar locMat[9][9], locVec[9];
    Node *alph_ptr, *omeg_ptr, *beta_ptr;
    PetscInt N;

    void SetUp()
    {
        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetFromOptions(glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,9,9);
        MatSetUp(glbMat);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,9);

        locMat[0][0] = 0;
        locVec[0] = 0;

        const char fileToRead[] = "data/exAssembleSystem.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        omeg_ptr = &(box_ptr->masterNodeList[1]);
        beta_ptr = &(box_ptr->masterNodeList[2]);

        omeg_ptr->nodeType = 0;

        N = box_ptr->nodeInternalCount;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        MatDestroy(&glbMat);
        VecDestroy(&glbVec);
    }
};


TEST_F(testAddBendContToGlobal, testErrorOutputAlphInternalBetaInternal)
{
    beta_ptr->nodeType = 0, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphInternalBetaDangling)
{
    beta_ptr->nodeType = 1, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphInternalBetaBoundary)
{
    beta_ptr->nodeType = 2, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphDanglingBetaInternal)
{
    beta_ptr->nodeType = 0, alph_ptr->nodeType = 1;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphDanglingBetaDangling)
{
    beta_ptr->nodeType = 1, alph_ptr->nodeType = 1;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphDanglingBetaBoundary)
{
    beta_ptr->nodeType = 2, alph_ptr->nodeType = 1;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphBoundarylBetaInternal)
{
    beta_ptr->nodeType = 0, alph_ptr->nodeType = 2;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphBoundarylBetaDangling)
{
    beta_ptr->nodeType = 1, alph_ptr->nodeType = 2;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphBoundarylBetaBoundary)
{
    beta_ptr->nodeType = 2, alph_ptr->nodeType = 2;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputOmegNotInternal)
{
    omeg_ptr->nodeType = 1;
    beta_ptr->nodeType = 0, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
    beta_ptr->nodeType = 1, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
    beta_ptr->nodeType = 2, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);

    omeg_ptr->nodeType = 2;
    beta_ptr->nodeType = 0, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
    beta_ptr->nodeType = 1, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
    beta_ptr->nodeType = 2, alph_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(glbMat,glbVec,N,locMat,locVec,alph_ptr,omeg_ptr,beta_ptr), 0);
}

} /* namespace */
