#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "systemAssembly.h"
}


namespace {

TEST(testReadInt, testRowFileReadIn) 
{
    PetscInt array[9];
    char fileName[100] = "../../data/row/idxm.lmbTripod1";
    readInt(fileName, array, 9);

    EXPECT_EQ(array[0], 0);
    EXPECT_EQ(array[1], 0);
    EXPECT_EQ(array[2], 0);

    EXPECT_EQ(array[3], 1);
    EXPECT_EQ(array[4], 1);
    EXPECT_EQ(array[5], 1);

    EXPECT_EQ(array[6], 2);
    EXPECT_EQ(array[7], 2);
    EXPECT_EQ(array[8], 2);
}

TEST(testReadInt, testColFileReadIn) 
{
    PetscInt array[9];
    char fileName[100] = "../../data/col/col.lmbTripod1";
    readInt(fileName, array, 9);

    EXPECT_EQ(array[0], 0);
    EXPECT_EQ(array[1], 1);
    EXPECT_EQ(array[2], 2);

    EXPECT_EQ(array[3], 0);
    EXPECT_EQ(array[4], 1);
    EXPECT_EQ(array[5], 2);

    EXPECT_EQ(array[6], 0);
    EXPECT_EQ(array[7], 1);
    EXPECT_EQ(array[8], 2);
}

TEST(testReadDbl, testMatFileReadIn) 
{
    PetscScalar array[9];
    char fileName[100] = "../../data/mat/mat.lmbTripod1";
    readDbl(fileName, array, 9);

    EXPECT_DOUBLE_EQ(array[0],  0.0003959445836206);
    EXPECT_DOUBLE_EQ(array[1], -0.0001075029206406);
    EXPECT_DOUBLE_EQ(array[2],  0.0000000000000000);

    EXPECT_DOUBLE_EQ(array[3], -0.0001075029206406);
    EXPECT_DOUBLE_EQ(array[4],  0.0015832388287110);
    EXPECT_DOUBLE_EQ(array[5],  0.0000000000000000);

    EXPECT_DOUBLE_EQ(array[6],  0.0000000000000000);
    EXPECT_DOUBLE_EQ(array[7],  0.0000000000000000);
    EXPECT_DOUBLE_EQ(array[8],  0.0001711864792787);
}

TEST(testReadDbl, testRhsFileReadIn) 
{
    PetscScalar array[3];
    char fileName[100] = "../../data/vec/vec.lmbTripod1";
    readDbl(fileName, array, 3);

    EXPECT_DOUBLE_EQ(array[0],  0.0000056234484900);
    EXPECT_DOUBLE_EQ(array[1], -0.0000112379052171);
    EXPECT_DOUBLE_EQ(array[2],  0.0000000000000000);
}

TEST(testWriteDbl, testSolFileWriteOut) 
{
    PetscScalar array[3];
    char fileName[100] = "../../data/sol/sol.lmbTripod1";

    array[0] =  1.23456;
    array[1] =  7.80000;
    array[2] = -9.87654;

    writeDbl(fileName, array, 3);

    EXPECT_DOUBLE_EQ(array[0],  1.23456);
    EXPECT_DOUBLE_EQ(array[1],  7.80000);
    EXPECT_DOUBLE_EQ(array[2], -9.87654);
}

struct testTripodAssembly : ::testing::Test
{
    PetscErrorCode ierr;
    char rowFile[100] = "../../data/row/row.lmbTripod1";
    char colFile[100] = "../../data/col/col.lmbTripod1";
    char matFile[100] = "../../data/mat/mat.lmbTripod1";
    char vecFile[100] = "../../data/vec/vec.lmbTripod1";
    char solFile[100] = "../../data/sol/sol.lmbTripod1";

    void SetUp()
    {
        /* set up */
    }

    void TearDown()
    {
        /* clean up */
    }
};

TEST_F(testTripodAssembly, testIfRuns)
{
    ierr = tripodAssembly(rowFile,colFile,matFile,vecFile,solFile);
    EXPECT_EQ(ierr, 0);
}

TEST_F(testTripodAssembly, testSolFileOutput)
{
    PetscScalar array[3];

    tripodAssembly(rowFile,colFile,matFile,vecFile,solFile);

    readDbl(solFile, array, 3);

    EXPECT_DOUBLE_EQ(array[0], 1.0);
    EXPECT_DOUBLE_EQ(array[1], 1.0);
    EXPECT_DOUBLE_EQ(array[2], 1.0);
}

} /* namespace */
