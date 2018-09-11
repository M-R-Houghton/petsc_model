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
    char line[100], *lineCrop_ptr;

    void SetUp()
    {
        // open file, read in line and close
        const char *fileName = "data/exReadBoxLine.dat";
        FILE *fp             = fopen(fileName, "r");
        char *line_ptr       = fgets(line, sizeof(line), fp);
        fclose(fp);

        // crop initial character and move pointer
        char *tkn_ptr        = strtok(line_ptr, " ");
        lineCrop_ptr         = tkn_ptr + 2;
    }

    void TearDown()
    {

    }
};


TEST_F(testReadBoxLine, testErrorOutput)
{
    EXPECT_EQ(readBoxLine(lineCrop_ptr, &box_ptr), 0);
    destroyBox(box_ptr);
}


TEST_F(testReadBoxLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    readBoxLine(lineCrop_ptr, &box_ptr);

    EXPECT_EQ(box_ptr->xyzDimension[0], 1);
    EXPECT_EQ(box_ptr->xyzDimension[1], 2);
    EXPECT_EQ(box_ptr->xyzDimension[2], 3);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],  1);

    destroyBox(box_ptr);
}


struct testReadFibreLine : ::testing::Test
{
    Box  *box_ptr;
    char line[100], *lineCrop_ptr;

    void SetUp()
    {
        // open file, read in line and close
        const char *fileName = "data/exReadFibreLine.dat";
        FILE *fp             = fopen(fileName, "r");
        char *line_ptr       = fgets(line, sizeof(line), fp);
        fclose(fp);

        // crop initial character and move pointer
        char *tkn_ptr        = strtok(line_ptr, " ");
        lineCrop_ptr         = tkn_ptr + 2;

        // additional setup
        box_ptr = makeBox(4,5,1,2,3,1,1,1);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testReadFibreLine, testErrorOutput)
{
    EXPECT_EQ(readFibreLine(lineCrop_ptr, box_ptr), 0);
}


TEST_F(testReadFibreLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    /* set up test */
    Node *node0_ptr = &(box_ptr->masterNodeList[0]);
    Node *node1_ptr = &(box_ptr->masterNodeList[1]);
    Node *node2_ptr = &(box_ptr->masterNodeList[2]);
    Node *node3_ptr = &(box_ptr->masterNodeList[3]);

    readFibreLine(lineCrop_ptr, box_ptr);

    /* test fibre values */
    EXPECT_EQ(box_ptr->masterFibreList[2].fibreID,              2);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibre,         3);
    EXPECT_EQ(box_ptr->masterFibreList[2].radius,               0.46);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibreList[0],  node0_ptr);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibreList[1],  node3_ptr);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibreList[2],  node2_ptr);

    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[0],  node1_ptr);
    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[1],  node1_ptr);
    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[2],  node1_ptr);
}


TEST(testTrimRightWhitespace, testOutputValue)
{
    // set up comparison strings
    char exampleString1[100] = "some example string";
    char exampleString2[100] = "a_different string_ex";

    EXPECT_STRNE(exampleString1, exampleString2);

    // set up test strings to be trimmed
    char withSpace1[100] = "some example string        ";
    char withSpace2[100] = "a_different string_ex         ";

    EXPECT_STRNE(exampleString1, withSpace1);
    EXPECT_STRNE(exampleString2, withSpace2);

    // trim strings
    char *noSpace1 = trimRightWhitespace(withSpace1);
    char *noSpace2 = trimRightWhitespace(withSpace2);

    EXPECT_STREQ(exampleString1, noSpace1);
    EXPECT_STREQ(exampleString2, noSpace2);
}


struct testReadNodeLine : ::testing::Test
{
    Box         *box_ptr;
    PetscInt    *gIndex_ptr, gIndex;
    char        line[100], *lineCrop_ptr;

    void SetUp()
    {
        // open file, read in line and close
        const char *fileName = "data/exReadNodeLine.dat";
        FILE *fp             = fopen(fileName, "r");
        char *line_ptr       = fgets(line, sizeof(line), fp);
        fclose(fp);

        // crop initial character and move pointer
        char *tkn_ptr        = strtok(line_ptr, " ");
        lineCrop_ptr         = tkn_ptr + 2;

        // additional setup
        box_ptr     = makeBox(4,1,1,2,3,1,1,1);
        gIndex      = 0;
        gIndex_ptr  = &gIndex;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testReadNodeLine, testErrorOutput)
{
    EXPECT_EQ(readNodeLine(lineCrop_ptr, box_ptr, gIndex_ptr, 0.05), 0);
}


TEST_F(testReadNodeLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    readNodeLine(lineCrop_ptr, box_ptr, gIndex_ptr, 0.05);

    EXPECT_EQ(box_ptr->masterNodeList[3].nodeID,    3);
    EXPECT_EQ(box_ptr->masterNodeList[3].nodeType,  2);

    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzCoord[0], 0.25);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzCoord[1], 0.00);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzCoord[2], 0.75);

    EXPECT_EQ(box_ptr->masterNodeList[3].globalID, -1);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzDisplacement[0], 0.00);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzDisplacement[1], 0.00);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzDisplacement[2], 0.00);
}

} /* namespace */
