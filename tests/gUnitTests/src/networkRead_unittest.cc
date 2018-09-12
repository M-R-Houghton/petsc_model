#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C"
{
    #include "networkRead.h"
}


namespace {


struct testNetworkRead : ::testing::Test
{
    Box  *box_ptr;
    const char *fileToRead;

    void SetUp()
    {
        // open file, read in line and close
        fileToRead = "data/exReadNetwork.dat";
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testNetworkRead, testErrorOutput)
{
    EXPECT_EQ(networkRead(fileToRead, &box_ptr, 0.05), 0);
}


TEST_F(testNetworkRead, testReadValues)
{
    EXPECT_EQ(networkRead(fileToRead, &box_ptr, 0.05), 1);
}


struct testReadDataLine : ::testing::Test
{
    Box         *box_ptr;
    char        bLine[100], fLine[100], nLine[100];
    char        *box_line_ptr, *fibre_line_ptr, *node_line_ptr;
    PetscInt    *gIndex_ptr, gIndex;

    void SetUp()
    {
        // open box line file, read in line and close
        const char *bFileName = "data/exReadBoxLine.dat";
        FILE *f1_ptr          = fopen(bFileName, "r");
        box_line_ptr          = fgets(bLine, sizeof(bLine), f1_ptr);
        fclose(f1_ptr);

        // open fibre line file, read in line and close
        const char *fFileName = "data/exReadFibreLine.dat";
        FILE *f2_ptr          = fopen(fFileName, "r");
        fibre_line_ptr        = fgets(fLine, sizeof(fLine), f2_ptr);
        fclose(f2_ptr);

        // open node line file, read in line and close
        const char *nFileName = "data/exReadNodeLine.dat";
        FILE *f3_ptr          = fopen(nFileName, "r");
        node_line_ptr         = fgets(nLine, sizeof(nLine), f3_ptr);
        fclose(f3_ptr);

        // additional setup
        gIndex      = 0;
        gIndex_ptr  = &gIndex;
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testReadDataLine, testErrorOutput)
{
    EXPECT_EQ(readDataLine(box_line_ptr, &box_ptr, gIndex_ptr, 0.045), 0);
    EXPECT_EQ(readDataLine(fibre_line_ptr, &box_ptr, gIndex_ptr, 0.3), 0);
    EXPECT_EQ(readDataLine(node_line_ptr, &box_ptr, gIndex_ptr, 0.02), 0);
}


TEST_F(testReadDataLine, testReadBoxValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    readDataLine(box_line_ptr, &box_ptr, gIndex_ptr, 0.045);
    readDataLine(fibre_line_ptr, &box_ptr, gIndex_ptr, 0.3);
    readDataLine(node_line_ptr, &box_ptr, gIndex_ptr, 0.02);

    EXPECT_EQ(box_ptr->nodeCount,              5);
    EXPECT_EQ(box_ptr->fibreCount,             4);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[0], 1.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[1], 2.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[2], 3.0);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],         1);
}


TEST_F(testReadDataLine, testReadFibreValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    readDataLine(box_line_ptr, &box_ptr, gIndex_ptr, 0.045);
    readDataLine(fibre_line_ptr, &box_ptr, gIndex_ptr, 0.3);
    readDataLine(node_line_ptr, &box_ptr, gIndex_ptr, 0.02);

    /* set up test */
    Node *node0_ptr = &(box_ptr->masterNodeList[0]);
    Node *node1_ptr = &(box_ptr->masterNodeList[1]);
    Node *node2_ptr = &(box_ptr->masterNodeList[2]);
    Node *node3_ptr = &(box_ptr->masterNodeList[3]);

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


TEST_F(testReadDataLine, testReadNodeValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    readDataLine(box_line_ptr, &box_ptr, gIndex_ptr, 0.045);
    readDataLine(fibre_line_ptr, &box_ptr, gIndex_ptr, 0.3);
    readDataLine(node_line_ptr, &box_ptr, gIndex_ptr, 0.02);

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


struct testReadBoxLine : ::testing::Test
{
    Box  *box_ptr;
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
        destroyBox(box_ptr);
    }
};


TEST_F(testReadBoxLine, testErrorOutput)
{
    EXPECT_EQ(readBoxLine(lineCrop_ptr, &box_ptr), 0);
}


TEST_F(testReadBoxLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    readBoxLine(lineCrop_ptr, &box_ptr);

    EXPECT_EQ(box_ptr->nodeCount,              5);
    EXPECT_EQ(box_ptr->fibreCount,             4);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[0], 1.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[1], 2.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[2], 3.0);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],         1);
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
