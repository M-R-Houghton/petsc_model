#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "systemAssembly.h"
}


namespace {

TEST(testReadInt, testFileOpen) 
{
    PetscInt array[9];
    readInt("../../data/row/idxm.lmbTripod1", array, 9);

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

TEST(testReadInt, testFileClose) 
{

}

TEST(testReadDbl, testFileOpen) 
{

}

TEST(testReadDbl, testFileClose) 
{

}

} /* namespace */
