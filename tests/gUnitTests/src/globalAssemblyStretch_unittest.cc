#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "globalAssemblyStretch.h"
}


namespace {

struct testAddSingleStretchCont : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat testMatrix;
    Vec testVector;
    PetscScalar locMat[6][6];

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


TEST_F(testAddSingleStretchCont, testErrorOutput) 
{
    EXPECT_EQ(addSingleStretchCont(box_ptr,testMatrix,locMat, 0,0,0,0), 0);
    EXPECT_EQ(addSingleStretchContFAST(box_ptr,testMatrix,locMat, 0,0,0,0), 0);
}


struct testAddStretchContToGlobal : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat glbMat;
    Vec glbVec;
    PetscScalar locMat[6][6], locVec[6];
    Node *alpha_ptr, *beta_ptr;

    void SetUp()
    {
        glbMat = NULL;
        glbVec = NULL;

        locMat[0][0] = 0;
        locVec[0] = 0;

        const char fileToRead[] = "data/exReadNetwork.dat";
        networkRead(fileToRead, &box_ptr, 0.05);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testAddStretchContToGlobal, testErrorOutput)
{
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
}

} /* namespace */