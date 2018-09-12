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
    const char *fileToWrite;
    void SetUp()
    {
        fileToWrite  = "data/exWriteBoxLine.dat";
        box_ptr   = makeBox(4,5,1,2,3,1,1,1);
    }

    void TearDown()
    {
        /* clean up */
        destroyBox(box_ptr);
    }
};


TEST_F(testWriteBoxLine, testErrorOutput)
{
    fp = fopen(fileToWrite, "w");
    EXPECT_EQ(writeBoxLine(fp, box_ptr), 1);
    fclose(fp);
}

TEST_F(testWriteBoxLine, testWriteBoxValues)
{
    fp = fopen(fileToWrite, "w");
    writeBoxLine(fp, box_ptr)
    fclose(fp);

    networkRead(fileToWrite, &box_ptr, 0.05);

    EXPECT_EQ(box_ptr->nodeCount,              5);
    EXPECT_EQ(box_ptr->fibreCount,             4);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[0], 1.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[1], 2.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[2], 3.0);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],         1);
}

} /* namespace */
