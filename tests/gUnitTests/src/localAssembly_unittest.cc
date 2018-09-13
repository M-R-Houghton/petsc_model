#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "localAssembly.h"
}


namespace {

struct testAddLocalContributions : ::testing::Test
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

TEST_F(testAddLocalContributions, testErrorOutput) 
{
    EXPECT_EQ(addLocalContributions(box_ptr, par_ptr, testMatrix, testVector), 0);
}

} /* namespace */