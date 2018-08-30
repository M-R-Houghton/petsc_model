#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "networkRead.h"
}


namespace {

struct testReadBoxLine : ::testing::Test
{
    Box *box_ptr;
    FILE *fp;
    const char *fileName;
    void SetUp()
    {
        fileName                    = "data/exReadBoxLine.dat";
        box_ptr                     = (Box *)malloc(sizeof(Box));
        box_ptr->xyzDimension[0]    = 0;
        box_ptr->xyzDimension[1]    = 0;
        box_ptr->xyzDimension[2]    = 0;
        box_ptr->xyzPeriodic[0]     = 0;
        box_ptr->xyzPeriodic[1]     = 0;
        box_ptr->xyzPeriodic[2]     = 0;
    }

    void TearDown()
    {
        /* clean up */
        free(box_ptr); box_ptr = NULL;
    }
};


TEST_F(testReadBoxLine, testErrorOutput)
{
    fp = fopen(fileName, "r");
    EXPECT_EQ(readBoxLine(fp, box_ptr), 1);
    fclose(fp);
}


TEST_F(testReadBoxLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    fp = fopen(fileName, "r");
    readBoxLine(fp, box_ptr);

    EXPECT_EQ(box_ptr->xyzDimension[0], 1);
    EXPECT_EQ(box_ptr->xyzDimension[1], 2);
    EXPECT_EQ(box_ptr->xyzDimension[2], 3);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],  1);
    
    fclose(fp);
}

} /* namespace */
