#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "networkWrite.h"
}


namespace {

struct testWriteBoxLine : ::testing::Test
{
    Box *box_ptr;
    FILE *fp;
    const char *fileName;
    void SetUp()
    {
        fileName                    = "data/exWriteBoxLine.dat";
        box_ptr                     = (Box *)malloc(sizeof(Box));
        box_ptr->xyzDimension[0]    = 1;
        box_ptr->xyzDimension[1]    = 2;
        box_ptr->xyzDimension[2]    = 3;
        box_ptr->xyzPeriodic[0]     = 1;
        box_ptr->xyzPeriodic[1]     = 1;
        box_ptr->xyzPeriodic[2]     = 1;
    }

    void TearDown()
    {
        /* clean up */
        free(box_ptr); box_ptr = NULL;
    }
};


TEST_F(testWriteBoxLine, testErrorOutput)
{
    ASSERT_TRUE(DIMENSION == 3);

    fp = fopen(fileName, "w");

    EXPECT_EQ(writeBoxLine(fp, box_ptr), 1);

    fclose(fp);
}

} /* namespace */
