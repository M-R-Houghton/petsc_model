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

TEST_F(testAddFibreLocalStretch, testErrorOutput) 
{
    EXPECT_EQ(addFibreLocalStretch(box_ptr, par_ptr, testMatrix, testVector, 0), 0);
    EXPECT_EQ(addFibreLocalStretch(box_ptr, par_ptr, testMatrix, testVector, 1), 0);
    EXPECT_EQ(addFibreLocalStretch(box_ptr, par_ptr, testMatrix, testVector, 2), 0);
}


struct testMake2DStretchMatrix : ::testing::Test
{
    void SetUp()
    {
        
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake2DStretchMatrix, DISABLED_testOutputValues)
{
	ASSERT_TRUE(DIMENSION == 2);
}


struct testMake3DStretchMatrix : ::testing::Test
{
    void SetUp()
    {
        
    }

    void TearDown()
    {
        
    }
};


TEST_F(testMake3DStretchMatrix, testOutputValues)
{
	ASSERT_TRUE(DIMENSION == 3);
}



} /* namespace */


