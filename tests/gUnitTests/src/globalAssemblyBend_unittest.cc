#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "globalAssemblyBend.h"
}


namespace {

struct testAddSingleBendCont : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat testMatrix;
    Vec testVector;

    void SetUp()
    {
        testMatrix = NULL;
        testVector = NULL;

        const char fileToRead[] = "data/exReadNetwork.dat";
        networkRead(fileToRead, &box_ptr, 0.05);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testAddSingleBendCont, testErrorOutput) 
{
    EXPECT_EQ(addSingleBendCont(), 0);
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
    EXPECT_EQ(addVecSingleBendCont(), 0);
}


struct testAddBendContToGlobal : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat glbMat;
    Vec glbVec;
    PetscScalar locMat[9][9], locVec[9];
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
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphInternalBetaDangling)
{
    beta_ptr->nodeType = 1, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphInternalBetaBoundary)
{
    beta_ptr->nodeType = 2, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphDanglingBetaInternal)
{
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphDanglingBetaDangling)
{
    beta_ptr->nodeType = 1, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphDanglingBetaBoundary)
{
    beta_ptr->nodeType = 2, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphBoundarylBetaInternal)
{
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphBoundarylBetaDangling)
{
    beta_ptr->nodeType = 1, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addBendContToGlobal(), 0);
}


TEST_F(testAddBendContToGlobal, testErrorOutputAlphBoundarylBetaBoundary)
{
    beta_ptr->nodeType = 2, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addBendContToGlobal(), 0);
}

} /* namespace */
