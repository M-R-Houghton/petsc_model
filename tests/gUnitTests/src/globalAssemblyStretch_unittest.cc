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
    Mat glbMat;
    Vec glbVec;
    PetscScalar locMat[6][6];

    void SetUp()
    {
        glbVec = NULL;

        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        VecCreate(PETSC_COMM_WORLD,&glbVec);
        VecSetSizes(glbVec,PETSC_DECIDE,6);

        MatCreate(PETSC_COMM_WORLD,&glbMat);
        MatSetSizes(glbMat,PETSC_DECIDE,PETSC_DECIDE,3,3);
        MatSetUp(glbMat);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testAddSingleStretchCont, testErrorOutput) 
{
    EXPECT_EQ(addSingleStretchCont(box_ptr,glbMat,locMat,0,0,0,0), 0);
    //EXPECT_EQ(addSingleStretchContFAST(box_ptr,glbMat,locMat, 0,0,0,0), 0);
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

        alpha_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr = &(box_ptr->masterNodeList[1]);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testAddStretchContToGlobal, testErrorOutput)
{
    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 0;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
    beta_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
    beta_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);

    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
    beta_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
    beta_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);

    beta_ptr->nodeType = 0, alpha_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
    beta_ptr->nodeType = 1;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
    beta_ptr->nodeType = 2;
    EXPECT_EQ(addStretchContToGlobal(alpha_ptr,beta_ptr,glbMat,glbVec,locMat,locVec), 0);
}

} /* namespace */