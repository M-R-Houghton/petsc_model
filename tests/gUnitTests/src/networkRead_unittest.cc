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


TEST_F(testNetworkRead, testNodeInternalCount)
{
    networkRead(fileToRead, &box_ptr, 0.05);

    EXPECT_EQ(box_ptr->nodeInternalCount, 2);
}


TEST_F(testNetworkRead, testReadBoxValues)
{
    networkRead(fileToRead, &box_ptr, 0.05);

    EXPECT_EQ(box_ptr->nodeCount,              5);
    EXPECT_EQ(box_ptr->fibreCount,             4);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[0], 1.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[1], 2.0);
    EXPECT_DOUBLE_EQ(box_ptr->xyzDimension[2], 3.0);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],         1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],         1);
}


TEST_F(testNetworkRead, testReadFibreValues)
{
    networkRead(fileToRead, &box_ptr, 0.05);

    // set up test
    Node *node0_ptr = &(box_ptr->masterNodeList[0]);
    Node *node1_ptr = &(box_ptr->masterNodeList[1]);
    Node *node2_ptr = &(box_ptr->masterNodeList[2]);
    Node *node3_ptr = &(box_ptr->masterNodeList[3]);
    Node *node4_ptr = &(box_ptr->masterNodeList[4]);

    // test fibre values
    EXPECT_EQ(box_ptr->masterFibreList[2].fibreID,              2);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibre,         3);
    EXPECT_EQ(box_ptr->masterFibreList[2].radius,               0.46);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibreList[0],  node0_ptr);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibreList[1],  node3_ptr);
    EXPECT_EQ(box_ptr->masterFibreList[2].nodesOnFibreList[2],  node2_ptr);

    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[0],  node1_ptr);
    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[1],  node1_ptr);
    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[2],  node1_ptr);

    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[0],  node4_ptr);
    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[1],  node4_ptr);
    EXPECT_NE(box_ptr->masterFibreList[2].nodesOnFibreList[2],  node4_ptr);

    EXPECT_EQ(box_ptr->masterFibreList[0].nodesOnFibre,         2);
    EXPECT_EQ(box_ptr->masterFibreList[1].nodesOnFibre,         2);
    EXPECT_EQ(box_ptr->masterFibreList[3].nodesOnFibre,         3);
}


TEST_F(testNetworkRead, testReadNodeValues)
{
    networkRead(fileToRead, &box_ptr, 0.05);

    EXPECT_EQ(box_ptr->masterNodeList[3].nodeID,    3);
    EXPECT_EQ(box_ptr->masterNodeList[3].nodeType,  0);

    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzCoord[0], 0.25);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzCoord[1], 0.5);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzCoord[2], 0.7);

    EXPECT_EQ(box_ptr->masterNodeList[3].globalID, 1);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzDisplacement[0], 0.0);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzDisplacement[1], 0.0);
    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[3].xyzDisplacement[2], 0.0);

    EXPECT_DOUBLE_EQ(box_ptr->masterNodeList[0].xyzDisplacement[0], 0.05);
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


struct testSetInternalNodeIndices : ::testing::Test
{
    Box  *box_ptr;
	PetscInt gIndex;
	PetscInt *gIndex_ptr;

    void SetUp()
    {
        // open file, read in line and close
        const char fileToRead[] = "../../data/dat/tri/tri_3d_01_in.dat";
        networkRead(fileToRead, &box_ptr, 0.05);

	    /* declare array for storing line, pointer, and counter for current line */
	    char line[MAX_LENGTH], *line_ptr;
	    PetscInt line_number = 0;
        PetscScalar gamma = 1.0;
	    FILE *fp;

	    /* setup global index */
	    gIndex = 0;
	    gIndex_ptr = &gIndex;

	    /* open file and check whether successful */
	    fp = fopen(fileToRead, "r");
	    if (fp == NULL) FAIL();

	    /* read in line by line until EOF is reached */
	    while ((line_ptr = fgets(line, sizeof(line), fp)) != NULL)
	    {
	    	/* read in line and increment line number */
	    	readDataLine(line_ptr, &box_ptr, gIndex_ptr, gamma);
	    	line_number += 1;
	    }

	    /* use final global index to set total internal nodes */
	    box_ptr->nodeInternalCount = gIndex;

	    /* close file */
	    fclose(fp);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testSetInternalNodeIndices, testStandardOutput)
{
    EXPECT_EQ(setInternalNodeIndices(box_ptr, PETSC_FALSE, *gIndex_ptr), 1);
}


TEST_F(testSetInternalNodeIndices, testCoupledOutput)
{
    // This test fails the assertion. Force fail is cleaner until fixed.
    //EXPECT_EQ(setInternalNodeIndices(box_ptr, PETSC_TRUE, *gIndex_ptr), 0);
    FAIL();
}


TEST_F(testSetInternalNodeIndices, testNumberingBefore)
{
    for (int i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node = &(box_ptr->masterNodeList[i]);
        if (node->nodeType == NODE_INTERNAL)
        {
            EXPECT_EQ(node->globalID, -2);
        }
    }
}


TEST_F(testSetInternalNodeIndices, testStandardNumberingAfter)
{
    setStandardInternalNodeIndices(box_ptr, 0);
    for (int i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node = &(box_ptr->masterNodeList[i]);
        if (node->nodeType == NODE_INTERNAL)
        {
            EXPECT_NE(node->globalID, -2);
        }
    }
}


TEST_F(testSetInternalNodeIndices, DISABLED_testCoupledNumberingAfter)
{
    //setStandardInternalNodeIndices(box_ptr, 0);
    for (int i = 0; i < box_ptr->nodeCount; i++)
    {
        Node *node = &(box_ptr->masterNodeList[i]);
        if (node->nodeType == NODE_INTERNAL)
        {
            EXPECT_NE(node->globalID, -2);
        }
    }
}


struct testSetStandardInternalNodeIndices : ::testing::Test
{
    void SetUp()
    {

    }
    void TearDown()
    {

    }
};


TEST_F(testSetStandardInternalNodeIndices, testOutput)
{
    FAIL();
}


struct testSetCoupledInternalNodeIndices : ::testing::Test
{
    void SetUp()
    {

    }
    void TearDown()
    {

    }
};


TEST_F(testSetCoupledInternalNodeIndices, testOutput)
{
    FAIL();
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
    EXPECT_EQ(readNodeLine(lineCrop_ptr, box_ptr, 0.05), 0);
}


TEST_F(testReadNodeLine, testReadValues)
{
    readNodeLine(lineCrop_ptr, box_ptr, 0.05);

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


struct testReadCoupleLine : ::testing::Test
{
    Box *box_ptr;
    PetscInt cID;
    void SetUp()
    {
        
        box_ptr = makeBox(0,0,1,2,3,1,1,1);
    }

    void TearDown()
    {
        destroyBox(box_ptr);
    }
};


TEST_F(testReadCoupleLine, testErrorOutput)
{
    char cpl[] = "c 123 456 ";
    cID = 3;

    EXPECT_NE(box_ptr->masterFibreList, nullptr);
    EXPECT_NE(box_ptr->masterNodeList, nullptr);
    EXPECT_EQ(box_ptr->masterCoupleList, nullptr);

    box_ptr->masterCoupleList = (Couple*)calloc(5, sizeof(Couple));
    EXPECT_NE(box_ptr->masterCoupleList, nullptr);
    EXPECT_EQ(readCoupleLine(cpl, box_ptr, cID), 0);
}


TEST_F(testReadCoupleLine, testPair)
{
    EXPECT_EQ(box_ptr->masterCoupleList, nullptr);
    box_ptr->masterCoupleList = (Couple*)calloc(5, sizeof(Couple));
    EXPECT_NE(box_ptr->masterCoupleList, nullptr);

    char cpl2[] = "153 46";
    cID = 2;
    readCoupleLine(cpl2, box_ptr, cID);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].coupleID, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodesInCouple, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[0], 153);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[1], 46);

    char cpl1[] = " 582 673 ";
    cID = 1;
    readCoupleLine(cpl1, box_ptr, cID);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].coupleID, 1);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodesInCouple, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[0], 582);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[1], 673);

    char cpl4[] = "9 98421 ";
    cID = 4;
    readCoupleLine(cpl4, box_ptr, cID);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].coupleID, 4);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodesInCouple, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[0], 9);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[1], 98421);

    // re-check after further adding
    cID = 2;
    EXPECT_EQ(box_ptr->masterCoupleList[cID].coupleID, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodesInCouple, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[0], 153);
    EXPECT_EQ(box_ptr->masterCoupleList[cID].nodeID[1], 46);
}


TEST_F(testReadCoupleLine, testMultipleNodeCouples)
{
    box_ptr->masterCoupleList = (Couple*)calloc(4, sizeof(Couple));
    EXPECT_NE(box_ptr->masterCoupleList, nullptr);

    char cpl1[] = " 8 582 7 673 ";
    char cpl2[] = " 163 46 9 2 11";
    char cpl3[] = "9 98421 51 ";

    readCoupleLine(cpl2, box_ptr, 2);
    readCoupleLine(cpl1, box_ptr, 1);
    readCoupleLine(cpl3, box_ptr, 3);

    EXPECT_EQ(box_ptr->masterCoupleList[1].coupleID, 1);
    EXPECT_EQ(box_ptr->masterCoupleList[1].nodesInCouple, 4);
    EXPECT_EQ(box_ptr->masterCoupleList[1].nodeID[0],   8);
    EXPECT_EQ(box_ptr->masterCoupleList[1].nodeID[1], 582);
    EXPECT_EQ(box_ptr->masterCoupleList[1].nodeID[2],   7);
    EXPECT_EQ(box_ptr->masterCoupleList[1].nodeID[3], 673);

    EXPECT_EQ(box_ptr->masterCoupleList[2].coupleID, 2);
    EXPECT_EQ(box_ptr->masterCoupleList[2].nodesInCouple, 5);
    EXPECT_EQ(box_ptr->masterCoupleList[2].nodeID[0], 163);
    EXPECT_EQ(box_ptr->masterCoupleList[2].nodeID[1],  46);
    EXPECT_EQ(box_ptr->masterCoupleList[2].nodeID[2],   9);
    EXPECT_EQ(box_ptr->masterCoupleList[2].nodeID[3],   2);
    EXPECT_EQ(box_ptr->masterCoupleList[2].nodeID[4],  11);

    EXPECT_EQ(box_ptr->masterCoupleList[3].coupleID, 3);
    EXPECT_EQ(box_ptr->masterCoupleList[3].nodesInCouple, 3);
    EXPECT_EQ(box_ptr->masterCoupleList[3].nodeID[0],     9);
    EXPECT_EQ(box_ptr->masterCoupleList[3].nodeID[1], 98421);
    EXPECT_EQ(box_ptr->masterCoupleList[3].nodeID[2],    51);
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


} /* namespace */
