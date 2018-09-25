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
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
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
        destroyBox(box_ptr);
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
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
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
        destroyBox(box_ptr);
    }
};


TEST_F(testUpdateInternalNodeDisp, testErrorOutput)
{
    EXPECT_EQ(updateInternalNodeDisp(alph_ptr, box_ptr->nodeInternalCount, glbVec), 0);
}


struct testUpdateDanglingNodeDisp : ::testing::Test
{
    Box *box_ptr;
    Node *alph_ptr;
    Node *beta_ptr;
    Node *delt_ptr;

    void SetUp()
    {
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr = &(box_ptr->masterNodeList[4]);
        delt_ptr = &(box_ptr->masterNodeList[1]);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testUpdateDanglingNodeDisp, testErrorOutput)
{
    EXPECT_EQ(updateDanglingNodeDisp(box_ptr, alph_ptr, beta_ptr, delt_ptr), 0);
}


TEST_F(testUpdateDanglingNodeDisp, testOutputValues)
{
    box_ptr->xyzDimension[0] = 20.0;
    box_ptr->xyzDimension[1] = 20.0;
    box_ptr->xyzDimension[2] = 20.0;

    alph_ptr->xyzCoord[0] = 1.0;
    alph_ptr->xyzCoord[1] = 1.0;
    alph_ptr->xyzCoord[2] = 0.5;

    alph_ptr->xyzDisplacement[0] = 1.0;
    alph_ptr->xyzDisplacement[1] = 1.0;
    alph_ptr->xyzDisplacement[2] = 0.0;

    beta_ptr->xyzCoord[0] = 4.0;
    beta_ptr->xyzCoord[1] = 3.0;
    beta_ptr->xyzCoord[2] = 1.5;

    beta_ptr->xyzDisplacement[0] = 2.0;
    beta_ptr->xyzDisplacement[1] = 0.0;
    beta_ptr->xyzDisplacement[2] = 0.0;

    delt_ptr->xyzCoord[0] = 5.5;
    delt_ptr->xyzCoord[1] = 4.0;
    delt_ptr->xyzCoord[2] = 2.0;

    updateDanglingNodeDisp(box_ptr, alph_ptr, beta_ptr, delt_ptr);

    PetscScalar t_nalpBeta[DIMENSION], s_nalpBeta[DIMENSION];
    int i;
    for (i = 0; i < DIMENSION; i++)
    {
        s_nalpBeta[i] = (beta_ptr->xyzCoord[i] + beta_ptr->xyzDisplacement[i]) 
                        - (alph_ptr->xyzCoord[i] + alph_ptr->xyzDisplacement[i]);
    }

    makeTangentVec(t_nalpBeta, s_nalpBeta);

    PetscScalar s_nx = 6.0 + t_nalpBeta[0] * sqrt(pow(1.5,2)+pow(1.0,2)+pow(0.5,2));
    PetscScalar s_ny = 3.0 + t_nalpBeta[1] * sqrt(pow(1.5,2)+pow(1.0,2)+pow(0.5,2));
    PetscScalar s_nz = 1.5 + t_nalpBeta[2] * sqrt(pow(1.5,2)+pow(1.0,2)+pow(0.5,2));

    EXPECT_DOUBLE_EQ(delt_ptr->xyzCoord[0], 5.5);
    EXPECT_DOUBLE_EQ(delt_ptr->xyzCoord[1], 4.0);
    EXPECT_DOUBLE_EQ(delt_ptr->xyzCoord[2], 2.0);

    EXPECT_DOUBLE_EQ(delt_ptr->xyzDisplacement[0], s_nx - 5.5);
    EXPECT_DOUBLE_EQ(delt_ptr->xyzDisplacement[1], s_ny - 4.0);
    EXPECT_DOUBLE_EQ(delt_ptr->xyzDisplacement[2], s_nz - 2.0);

    EXPECT_DOUBLE_EQ(delt_ptr->xyzCoord[0]+delt_ptr->xyzDisplacement[0], s_nx);
    EXPECT_DOUBLE_EQ(delt_ptr->xyzCoord[1]+delt_ptr->xyzDisplacement[1], s_ny);
    EXPECT_DOUBLE_EQ(delt_ptr->xyzCoord[2]+delt_ptr->xyzDisplacement[2], s_nz);
}

} /* namespace */
