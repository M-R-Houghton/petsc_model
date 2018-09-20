#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "networkUpdate.h"
}


namespace {

struct testNetworkUpdate : ::testing::Test
{
    Mat glbMat;
    Vec glbVec;
    Box *box_ptr;

    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,2);

        VecSet(glbVec,1.0);
    }

    void TearDown()
    {
        /* clean up */
        VecDestroy(&glbVec);
    }
};


TEST_F(testNetworkUpdate, testErrorOutput)
{
    EXPECT_EQ(networkUpdate(box_ptr, glbVec), 0);
}


struct testUpdateInternalNodeDisp : ::testing::Test
{
    Mat glbMat;
    Vec glbVec;
    Box *box_ptr;
    Node *alph_ptr;

    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetFromOptions(glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,3);

        VecSet(glbVec,1.0);

        alph_ptr = &(box_ptr->masterNodeList[3]);
    }

    void TearDown()
    {
        /* clean up */
        VecDestroy(&glbVec);
    }
};


TEST_F(testUpdateInternalNodeDisp, testErrorOutput)
{
    EXPECT_EQ(updateInternalNodeDisp(alph_ptr, box_ptr->nodeInternalCount, glbVec), 0);
}

} /* namespace */
