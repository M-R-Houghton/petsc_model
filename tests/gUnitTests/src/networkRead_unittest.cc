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
    char line[100], *line_ptr;

    void SetUp()
    {
        fileName = "data/exReadBoxLine.dat";
    }

    void TearDown()
    {

    }
};


TEST_F(testReadBoxLine, testErrorOutput)
{
    fp = fopen(fileName, "r");

    line_ptr = fgets(line, sizeof(line), fp);
    EXPECT_EQ(readBoxLine(line_ptr, &box_ptr), 0);

    destroyBox(box_ptr);

    fclose(fp);
}


TEST_F(testReadBoxLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);
    fp = fopen(fileName, "r");

    line_ptr = fgets(line, sizeof(line), fp);
    readBoxLine(line_ptr, &box_ptr);

    EXPECT_EQ(box_ptr->xyzDimension[0], 1);
    EXPECT_EQ(box_ptr->xyzDimension[1], 2);
    EXPECT_EQ(box_ptr->xyzDimension[2], 3);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],  1);

    destroyBox(box_ptr);

    fclose(fp);
}


struct testReadNodeLine : ::testing::Test
{
    Box *box_ptr;
    FILE *fp;
    const char *fileName;
    char line[100], *line_ptr;

    void SetUp()
    {
        fileName = "data/exReadNodeLine.dat";
        box_ptr = makeBox(3,1,1,2,3,1,1,1);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testReadNodeLine, testErrorOutput)
{
    fp = fopen(fileName, "r");

    line_ptr = fgets(line, sizeof(line), fp);
    EXPECT_EQ(readNodeLine(line_ptr, &box_ptr), 0);

    fclose(fp);
}


TEST_F(testReadNodeLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);
    fp = fopen(fileName, "r");

    line_ptr = fgets(line, sizeof(line), fp);
    readNodeLine(line_ptr, &box_ptr);

    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].nodeID, 1);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].nodeType, 2);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].xyzCoord[0], 0.25);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].xyzCoord[1], 0.00);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].xyzCoord[2], 0.75);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].globalID, -1);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].xyzDisplacement[0], 0);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].xyzDisplacement[1], 0);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[2].xyzDisplacement[2], 0);

    fclose(fp);
}

} /* namespace */
