#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "globalAssemblyBend.h"
}


namespace {

struct testAddSingleBendContribution : ::testing::Test
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

TEST_F(testAddSingleBendContribution, testErrorOutput) 
{
    EXPECT_EQ(addSingleBendCont(testMatrix,0,0,0,0), 0);
}

} /* namespace */