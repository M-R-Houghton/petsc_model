#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "localAssemblyStretch.h"
}


namespace {

TEST(testCalculateK, testOutputKValue)
{
	Parameters *par_ptr 				= (Parameters *)malloc(sizeof(Parameters));
	par_ptr->youngsModulus 				= 1.0/(M_PI * pow(0.01, 2));

	PetscInt fCount 		   			= 2;
	Box *box_ptr 			   			= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0]			= 10;
	box_ptr->xyzDimension[1]			= 10;
	box_ptr->xyzDimension[2]			= 10;
	box_ptr->fibreCount 	   			= fCount;
	box_ptr->masterFibreList   			= (Fibre*)calloc(fCount, sizeof(Fibre));
	box_ptr->masterFibreList[0].fibreID = 0;
	box_ptr->masterFibreList[0].radius 	= 0.01;
	box_ptr->masterFibreList[1].fibreID = 1;
	box_ptr->masterFibreList[1].radius 	= 0.3;

	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 0, 0.25), 4.0 		);
	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 0, 0.50), 2.0 		);
	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 0, 0.75), 1.0/0.75	);
	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 0, 1.00), 1.0 		);

	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 1, 0.25), 900.0/0.25	);
	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 1, 0.50), 900.0/0.50	);
	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 1, 0.75), 900.0/0.75	);
	EXPECT_DOUBLE_EQ(calculateK(box_ptr, par_ptr, 1, 1.00), 900.0 	 	);

	/* clean up */
	free(par_ptr); par_ptr = NULL;
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
}


struct testAddFibreLocalStretch : ::testing::Test
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

        par_ptr = makeParameters(1.0, 1.0);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
        destroyParameters(par_ptr);
        MatDestroy(&testMatrix);
        VecDestroy(&testVector);
    }
};

TEST_F(testAddFibreLocalStretch, testErrorOutput) 
{
    EXPECT_EQ(addFibreLocalStretch(box_ptr, par_ptr, testMatrix, testVector, 0), 0);
    EXPECT_EQ(addFibreLocalStretch(box_ptr, par_ptr, testMatrix, testVector, 1), 0);
}


struct testMake2DStretchMat : ::testing::Test
{
    PetscScalar locMat[6][6];
    PetscScalar tangVec[DIMENSION];
    void SetUp()
    {
        tangVec[0] = 1.0;
        tangVec[1] = 2.0;
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake2DStretchMat, DISABLED_testOutputValues)
{
	ASSERT_TRUE(DIMENSION == 2);

    EXPECT_EQ(make2DStretchMat(1.0, tangVec, locMat), 0);
}


struct testMake2DStretchVec : ::testing::Test
{
    Box *box_ptr;
    Node *alpha_ptr;
    Node *beta_ptr;
    PetscScalar locVec[6];
    PetscScalar tangVec[DIMENSION];
    void SetUp()
    {
        tangVec[0] = 1.0;
        tangVec[1] = 2.0;

        const char fileToRead[] = "data/exReadNetwork.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alpha_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr  = &(box_ptr->masterNodeList[1]);
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake2DStretchVec, DISABLED_testOutputValues)
{
    ASSERT_TRUE(DIMENSION == 2);

    EXPECT_EQ(make2DStretchVec(alpha_ptr->xyzDisplacement, beta_ptr->xyzDisplacement, 1.0, tangVec, locVec), 0);
}


struct testMake3DStretchMat : ::testing::Test
{
    PetscScalar locMat[6][6];
    PetscScalar tangVec[DIMENSION];
    void SetUp()
    {
        tangVec[0] = 1.0;
        tangVec[1] = 2.0;
        tangVec[2] = 3.0;
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake3DStretchMat, testErrorOutput)
{
	ASSERT_TRUE(DIMENSION == 3);

    EXPECT_EQ(make3DStretchMat(1.0, tangVec, locMat), 0);
}


TEST_F(testMake3DStretchMat, testMatrixOutput1)
{
    ASSERT_TRUE(DIMENSION == 3);

    tangVec[0] = -0.4472135954999579;
    tangVec[1] =  0.8944271909999159;
    tangVec[2] =  0.0;

    make3DStretchMat(0.0005619851784833, tangVec, locMat);

    EXPECT_NEAR(locMat[0][0],  0.0001123970356967, 1e-16);
    EXPECT_NEAR(locMat[0][1], -0.0001123970356967, 1e-16);
    EXPECT_NEAR(locMat[0][2], -0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[0][3],  0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[0][4], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[0][5],  0.0000000000000000, 1e-16);

    EXPECT_NEAR(locMat[1][0], -0.0001123970356967, 1e-16);
    EXPECT_NEAR(locMat[1][1],  0.0001123970356967, 1e-16);
    EXPECT_NEAR(locMat[1][2],  0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[1][3], -0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[1][4],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[1][5], -0.0000000000000000, 1e-16);

    EXPECT_NEAR(locMat[2][0], -0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[2][1],  0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[2][2],  0.0004495881427866, 1e-16);
    EXPECT_NEAR(locMat[2][3], -0.0004495881427866, 1e-16);
    EXPECT_NEAR(locMat[2][4],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[2][5], -0.0000000000000000, 1e-16);

    EXPECT_NEAR(locMat[3][0],  0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[3][1], -0.0002247940713933, 1e-16);
    EXPECT_NEAR(locMat[3][2], -0.0004495881427866, 1e-16);
    EXPECT_NEAR(locMat[3][3],  0.0004495881427866, 1e-16);
    EXPECT_NEAR(locMat[3][4], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[3][5],  0.0000000000000000, 1e-16);

    EXPECT_NEAR(locMat[4][0], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[4][1],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[4][2],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[4][3], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[4][4],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[4][5], -0.0000000000000000, 1e-16);

    EXPECT_NEAR(locMat[5][0],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[5][1], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[5][2], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[5][3],  0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[5][4], -0.0000000000000000, 1e-16);
    EXPECT_NEAR(locMat[5][5],  0.0000000000000000, 1e-16);
}


TEST_F(testMake3DStretchMat, testMatrixOutput2)
{
    ASSERT_TRUE(DIMENSION == 3);

    tangVec[0] = 0.4082482904638631;
    tangVec[1] = 0.8164965809277261;
    tangVec[2] = 0.4082482904638631;

    make3DStretchMat(0.0005130199320647, tangVec, locMat);

    EXPECT_NEAR(locMat[0][0],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[0][1], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[0][2],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[0][3], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[0][4],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[0][5], -0.0000855033220108, 1e-16);

    EXPECT_NEAR(locMat[1][0], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[1][1],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[1][2], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[1][3],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[1][4], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[1][5],  0.0000855033220108, 1e-16);

    EXPECT_NEAR(locMat[2][0],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[2][1], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[2][2],  0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[2][3], -0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[2][4],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[2][5], -0.0001710066440216, 1e-16);

    EXPECT_NEAR(locMat[3][0], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[3][1],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[3][2], -0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[3][3],  0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[3][4], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[3][5],  0.0001710066440216, 1e-16);

    EXPECT_NEAR(locMat[4][0],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[4][1], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[4][2],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[4][3], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[4][4],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[4][5], -0.0000855033220108, 1e-16);

    EXPECT_NEAR(locMat[5][0], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[5][1],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[5][2], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[5][3],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[5][4], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[5][5],  0.0000855033220108, 1e-16);
}


TEST_F(testMake3DStretchMat, testMatrixOutput3)
{
    ASSERT_TRUE(DIMENSION == 3);

    tangVec[0] =  0.4082482904638631;
    tangVec[1] =  0.8164965809277261;
    tangVec[2] = -0.4082482904638631;

    make3DStretchMat(0.0005130199320647, tangVec, locMat);

    EXPECT_NEAR(locMat[0][0],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[0][1], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[0][2],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[0][3], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[0][4], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[0][5],  0.0000855033220108, 1e-16);

    EXPECT_NEAR(locMat[1][0], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[1][1],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[1][2], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[1][3],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[1][4],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[1][5], -0.0000855033220108, 1e-16);

    EXPECT_NEAR(locMat[2][0],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[2][1], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[2][2],  0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[2][3], -0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[2][4], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[2][5],  0.0001710066440216, 1e-16);

    EXPECT_NEAR(locMat[3][0], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[3][1],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[3][2], -0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[3][3],  0.0003420132880432, 1e-16);
    EXPECT_NEAR(locMat[3][4],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[3][5], -0.0001710066440216, 1e-16);

    EXPECT_NEAR(locMat[4][0], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[4][1],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[4][2], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[4][3],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[4][4],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[4][5], -0.0000855033220108, 1e-16);

    EXPECT_NEAR(locMat[5][0],  0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[5][1], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[5][2],  0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[5][3], -0.0001710066440216, 1e-16);
    EXPECT_NEAR(locMat[5][4], -0.0000855033220108, 1e-16);
    EXPECT_NEAR(locMat[5][5],  0.0000855033220108, 1e-16);
}


struct testMake3DStretchVec : ::testing::Test
{
    Box *box_ptr;
    Node *alpha_ptr;
    Node *beta_ptr;
    PetscScalar locVec[6];
    PetscScalar tangVec[DIMENSION];
    void SetUp()
    {
        tangVec[0] = 1.0;
        tangVec[1] = 2.0;
        tangVec[2] = 3.0;

        const char fileToRead[] = "data/exReadNetwork.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

        alpha_ptr = &(box_ptr->masterNodeList[0]);
        beta_ptr  = &(box_ptr->masterNodeList[1]);
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake3DStretchVec, testOutputValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    EXPECT_EQ(make3DStretchVec(alpha_ptr->xyzDisplacement, beta_ptr->xyzDisplacement, 1.0, tangVec, locVec), 0);
}

} /* namespace */


