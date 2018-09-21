#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "localAssemblyBend.h"
}


namespace {

TEST(testCalculateKappa, testOutputKappaValue) 
{
    Parameters *par_ptr                 = (Parameters *)malloc(sizeof(Parameters));
    par_ptr->youngsModulus              = 1.0/(M_PI * pow(0.01, 2));

    PetscInt fCount                     = 2;
    Box *box_ptr                        = (Box *)malloc(sizeof(Box));
    box_ptr->fibreCount                 = fCount;
    box_ptr->masterFibreList            = (Fibre*)calloc(fCount, sizeof(Fibre));
    box_ptr->masterFibreList[0].fibreID = 0;
    box_ptr->masterFibreList[0].radius  = 0.01;
    box_ptr->masterFibreList[1].fibreID = 1;
    box_ptr->masterFibreList[1].radius  = 0.3;

    EXPECT_DOUBLE_EQ(calculateKappa(box_ptr, par_ptr, 0), 0.000025);
    EXPECT_DOUBLE_EQ(calculateKappa(box_ptr, par_ptr, 1), 20.25);

    /* clean up */
    free(par_ptr); par_ptr = NULL;
    free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
}


struct testAddFibreLocalBend : ::testing::Test
{
    Box *box_ptr;
    Parameters *par_ptr;
    Mat testMatrix;
    Vec testVector;

    void SetUp()
    {
        MatCreate(PETSC_COMM_WORLD,&testMatrix);
        MatSetFromOptions(testMatrix);
        MatSetSizes(testMatrix,PETSC_DECIDE,PETSC_DECIDE,3,3);
        MatSetUp(testMatrix);

        VecCreate(PETSC_COMM_WORLD,&testVector);
        VecSetFromOptions(testVector);
        VecSetSizes(testVector,PETSC_DECIDE,3);

        const char fileToRead[] = "../../data/dat/f3tTripod1_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        par_ptr = makeParameters(fileToRead, fileToRead, 1.0, 1.0);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
        MatDestroy(&testMatrix);
        VecDestroy(&testVector);
    }
};


TEST_F(testAddFibreLocalBend, testErrorOutput) 
{
    EXPECT_EQ(addFibreLocalBend(box_ptr, par_ptr, testMatrix, testVector, 0), 0);
    EXPECT_EQ(addFibreLocalBend(box_ptr, par_ptr, testMatrix, testVector, 1), 0);
}


struct testMake2DBendMat : ::testing::Test
{
    PetscScalar locMat[9][9];
    PetscScalar s_alphOmeg[DIMENSION];
    PetscScalar s_omegBeta[DIMENSION];
    PetscScalar s_alphBeta[DIMENSION];
    void SetUp()
    {
        s_alphOmeg[0] = 1.0;
        s_alphOmeg[1] = 2.0;

        s_omegBeta[0] = 3.0;
        s_omegBeta[1] = 4.0;

        s_alphBeta[0] = 5.0;
        s_alphBeta[1] = 6.0;
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake2DBendMat, DISABLED_testOutputValues)
{
    ASSERT_TRUE(DIMENSION == 2);
    EXPECT_EQ(make2DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, 1.0, locMat), 0);
}


struct testMake2DBendVec : ::testing::Test
{
    Box *box_ptr;
    Node *alph_ptr;
    Node *omeg_ptr;
    Node *beta_ptr;
    PetscScalar locVec[9];
    PetscScalar s_alphOmeg[DIMENSION];
    PetscScalar s_omegBeta[DIMENSION];
    PetscScalar s_alphBeta[DIMENSION];
    void SetUp()
    {
        s_alphOmeg[0] = 1.0;
        s_alphOmeg[1] = 2.0;

        s_omegBeta[0] = 3.0;
        s_omegBeta[1] = 4.0;

        s_alphBeta[0] = 5.0;
        s_alphBeta[1] = 6.0;

        const char fileToRead[] = "data/exReadNetwork.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        omeg_ptr = &(box_ptr->masterNodeList[1]);
        beta_ptr = &(box_ptr->masterNodeList[2]);
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake2DBendVec, DISABLED_testOutputValues)
{
    ASSERT_TRUE(DIMENSION == 2);
    EXPECT_EQ(make2DBendVec(alph_ptr->xyzDisplacement, omeg_ptr->xyzDisplacement, 
                            beta_ptr->xyzDisplacement, s_alphOmeg, s_omegBeta, s_alphBeta, 1.0, locVec), 0);
}


struct testMake3DBendMat : ::testing::Test
{
    PetscScalar locMat[9][9];
    PetscScalar s_alphOmeg[DIMENSION];
    PetscScalar s_omegBeta[DIMENSION];
    PetscScalar s_alphBeta[DIMENSION];
    void SetUp()
    {
        s_alphOmeg[0] = -0.25;
        s_alphOmeg[1] =  0.50;
        s_alphOmeg[2] =  0.00;

        s_omegBeta[0] = -0.25;
        s_omegBeta[1] =  0.50;
        s_omegBeta[2] =  0.00;

        s_alphBeta[0] = -0.50;
        s_alphBeta[1] =  1.00;
        s_alphBeta[2] =  0.00;
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake3DBendMat, testErrorOutput)
{
    ASSERT_TRUE(DIMENSION == 3);

    s_alphOmeg[0] = 1.0;
    s_alphOmeg[1] = 2.0;
    s_alphOmeg[2] = 3.0;

    s_omegBeta[0] = 4.0;
    s_omegBeta[1] = 5.0;
    s_omegBeta[2] = 6.0;

    s_alphBeta[0] = 7.0;
    s_alphBeta[1] = 8.0;
    s_alphBeta[2] = 9.0;

    EXPECT_EQ(make3DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, 1.0, locMat), 0);
}


TEST_F(testMake3DBendMat, testMatrixOutputXRows)
{
    ASSERT_TRUE(DIMENSION == 3);

    make3DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, 1.43868205691714e-07, locMat);

    EXPECT_DOUBLE_EQ(locMat[0][0],  3.596705142292850e-08);
    EXPECT_DOUBLE_EQ(locMat[0][1], -7.193410284585700e-08);
    EXPECT_DOUBLE_EQ(locMat[0][2],  3.596705142292850e-08);
    EXPECT_DOUBLE_EQ(locMat[0][3],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[0][4], -3.596705142292850e-08);
    EXPECT_DOUBLE_EQ(locMat[0][5],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[0][6],  0.0);
    EXPECT_DOUBLE_EQ(locMat[0][7], -0.0);
    EXPECT_DOUBLE_EQ(locMat[0][8],  0.0);

    EXPECT_DOUBLE_EQ(locMat[1][0], -7.1934102845857e-08);
    EXPECT_DOUBLE_EQ(locMat[1][1],  1.43868205691714e-07);
    EXPECT_DOUBLE_EQ(locMat[1][2], -7.1934102845857e-08);
    EXPECT_DOUBLE_EQ(locMat[1][3], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[1][4],  7.1934102845857e-08);
    EXPECT_DOUBLE_EQ(locMat[1][5], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[1][6], -0.0);
    EXPECT_DOUBLE_EQ(locMat[1][7],  0.0);
    EXPECT_DOUBLE_EQ(locMat[1][8], -0.0);

    EXPECT_DOUBLE_EQ(locMat[2][0],  3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[2][1], -7.1934102845857e-08);
    EXPECT_DOUBLE_EQ(locMat[2][2],  3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[2][3],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[2][4], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[2][5],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[2][6],  0.0);
    EXPECT_DOUBLE_EQ(locMat[2][7], -0.0);
    EXPECT_DOUBLE_EQ(locMat[2][8],  0.0);
}


TEST_F(testMake3DBendMat, testMatrixOutputYRows)
{
    ASSERT_TRUE(DIMENSION == 3);

    make3DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, 1.43868205691714e-07, locMat);

    EXPECT_DOUBLE_EQ(locMat[3][0],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[3][1], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[3][2],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[3][3],  8.991762855732125e-09);
    EXPECT_DOUBLE_EQ(locMat[3][4], -1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[3][5],  8.991762855732125e-09);
    EXPECT_DOUBLE_EQ(locMat[3][6], -0.0);
    EXPECT_DOUBLE_EQ(locMat[3][7],  0.0);
    EXPECT_DOUBLE_EQ(locMat[3][8], -0.0);

    EXPECT_DOUBLE_EQ(locMat[4][0], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[4][1],  7.1934102845857e-08);
    EXPECT_DOUBLE_EQ(locMat[4][2], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[4][3], -1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[4][4],  3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[4][5], -1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[4][6],  0.0);
    EXPECT_DOUBLE_EQ(locMat[4][7], -0.0);
    EXPECT_DOUBLE_EQ(locMat[4][8],  0.0);

    EXPECT_DOUBLE_EQ(locMat[5][0],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[5][1], -3.59670514229285e-08);
    EXPECT_DOUBLE_EQ(locMat[5][2],  1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[5][3],  8.991762855732125e-09);
    EXPECT_DOUBLE_EQ(locMat[5][4], -1.798352571146425e-08);
    EXPECT_DOUBLE_EQ(locMat[5][5],  8.991762855732125e-09);
    EXPECT_DOUBLE_EQ(locMat[5][6], -0.0);
    EXPECT_DOUBLE_EQ(locMat[5][7],  0.0);
    EXPECT_DOUBLE_EQ(locMat[5][8], -0.0);
}


TEST_F(testMake3DBendMat, testMatrixOutputZRows)
{
    ASSERT_TRUE(DIMENSION == 3);

    make3DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, 1.43868205691714e-07, locMat);

    EXPECT_DOUBLE_EQ(locMat[6][0],  0.0);
    EXPECT_DOUBLE_EQ(locMat[6][1], -0.0);
    EXPECT_DOUBLE_EQ(locMat[6][2],  0.0);
    EXPECT_DOUBLE_EQ(locMat[6][3], -0.0);
    EXPECT_DOUBLE_EQ(locMat[6][4],  0.0);
    EXPECT_DOUBLE_EQ(locMat[6][5], -0.0);
    EXPECT_DOUBLE_EQ(locMat[6][6],  4.495881427866062e-08);
    EXPECT_DOUBLE_EQ(locMat[6][7], -8.991762855732125e-08);
    EXPECT_DOUBLE_EQ(locMat[6][8],  4.495881427866062e-08);

    EXPECT_DOUBLE_EQ(locMat[7][0], -0.0);
    EXPECT_DOUBLE_EQ(locMat[7][1],  0.0);
    EXPECT_DOUBLE_EQ(locMat[7][2], -0.0);
    EXPECT_DOUBLE_EQ(locMat[7][3],  0.0);
    EXPECT_DOUBLE_EQ(locMat[7][4], -0.0);
    EXPECT_DOUBLE_EQ(locMat[7][5],  0.0);
    EXPECT_DOUBLE_EQ(locMat[7][6], -8.991762855732125e-08);
    EXPECT_DOUBLE_EQ(locMat[7][7],  1.798352571146425e-07);
    EXPECT_DOUBLE_EQ(locMat[7][8], -8.991762855732125e-08);

    EXPECT_DOUBLE_EQ(locMat[8][0],  0.0);
    EXPECT_DOUBLE_EQ(locMat[8][1], -0.0);
    EXPECT_DOUBLE_EQ(locMat[8][2],  0.0);
    EXPECT_DOUBLE_EQ(locMat[8][3], -0.0);
    EXPECT_DOUBLE_EQ(locMat[8][4],  0.0);
    EXPECT_DOUBLE_EQ(locMat[8][5], -0.0);
    EXPECT_DOUBLE_EQ(locMat[8][6],  4.495881427866062e-08);
    EXPECT_DOUBLE_EQ(locMat[8][7], -8.991762855732125e-08);
    EXPECT_DOUBLE_EQ(locMat[8][8],  7.1934102845857e-08);
}


struct testMake3DBendVec : ::testing::Test
{
    Box *box_ptr;
    Node *alph_ptr;
    Node *omeg_ptr;
    Node *beta_ptr;
    PetscScalar locVec[9];
    PetscScalar s_alphOmeg[DIMENSION];
    PetscScalar s_omegBeta[DIMENSION];
    PetscScalar s_alphBeta[DIMENSION];
    void SetUp()
    {
        s_alphOmeg[0] = -0.25;
        s_alphOmeg[1] =  0.50;
        s_alphOmeg[2] =  0.00;

        s_omegBeta[0] = -0.25;
        s_omegBeta[1] =  0.50;
        s_omegBeta[2] =  0.00;

        s_alphBeta[0] = -0.50;
        s_alphBeta[1] =  1.00;
        s_alphBeta[2] =  0.00;

        const char fileToRead[] = "data/exReadNetwork.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alph_ptr = &(box_ptr->masterNodeList[0]);
        omeg_ptr = &(box_ptr->masterNodeList[3]);
        beta_ptr = &(box_ptr->masterNodeList[1]);
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake3DBendVec, testErrorOutput)
{
    ASSERT_TRUE(DIMENSION == 3);

    s_alphOmeg[0] = 1.0;
    s_alphOmeg[1] = 2.0;
    s_alphOmeg[2] = 3.0;

    s_omegBeta[0] = 4.0;
    s_omegBeta[1] = 5.0;
    s_omegBeta[2] = 6.0;

    s_alphBeta[0] = 7.0;
    s_alphBeta[1] = 8.0;
    s_alphBeta[2] = 9.0;

    EXPECT_EQ(make3DBendVec(alph_ptr->xyzDisplacement, omeg_ptr->xyzDisplacement, 
                            beta_ptr->xyzDisplacement, s_alphOmeg, s_omegBeta, s_alphBeta, 1.0, locVec), 0);
}


TEST_F(testMake3DBendVec, testOutputValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    make3DBendVec(alph_ptr->xyzDisplacement, omeg_ptr->xyzDisplacement, beta_ptr->xyzDisplacement, 
                    s_alphOmeg, s_omegBeta, s_alphBeta, 1.43868205691714e-07, locVec);

    EXPECT_DOUBLE_EQ(locVec[0], -1.798352571146425e-09);
    EXPECT_DOUBLE_EQ(locVec[1],  3.59670514229285e-09);
    EXPECT_DOUBLE_EQ(locVec[2], -1.798352571146425e-09);
    EXPECT_DOUBLE_EQ(locVec[3], -8.991762855732126e-10);
    EXPECT_DOUBLE_EQ(locVec[4],  1.798352571146425e-09);
    EXPECT_DOUBLE_EQ(locVec[5], -8.991762855732126e-10);
    EXPECT_DOUBLE_EQ(locVec[6], -0.00000);
    EXPECT_DOUBLE_EQ(locVec[7],  0.00000);
    EXPECT_DOUBLE_EQ(locVec[8], -0.00000);
}

} /* namespace */
