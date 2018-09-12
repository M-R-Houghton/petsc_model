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
        box_ptr   = makeBox(5,4,1,2,3,1,1,1);
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
    EXPECT_EQ(writeBoxLine(fp, box_ptr), 0);
    fclose(fp);
}


TEST_F(testWriteBoxLine, testWriteBoxValues)
{
    fp = fopen(fileToWrite, "w");
    writeBoxLine(fp, box_ptr);
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


struct testWriteFibreLine : ::testing::Test
{
    Box *box_ptr;
    FILE *fp;
    const char *fileToWrite;
    PetscInt *gIndex_ptr, gIndex;

    void SetUp()
    {
        fileToWrite  = "data/exWriteFibreLine.dat";

        box_ptr = makeBox(3,2,3,4,5,1,1,1);

        // set up node list and make fibre
        Node **node_ptr_list = (Node**)calloc(3, sizeof(Node*));

        // change node in node list
        node_ptr_list[0] = &(box_ptr->masterNodeList[2]);
        node_ptr_list[1] = &(box_ptr->masterNodeList[0]);
        node_ptr_list[2] = &(box_ptr->masterNodeList[1]);

        // additional setup
        gIndex      = 0;
        gIndex_ptr  = &gIndex;

        // make sure node slots aren't empty
        makeNode(box_ptr,0,2,3,4,5,gIndex_ptr,0.25);
        makeNode(box_ptr,1,0,6,7,8,gIndex_ptr,0.25);
        makeNode(box_ptr,2,1,9,1,2,gIndex_ptr,0.25);

        makeFibre(box_ptr,1,3,0.05,node_ptr_list);
    }

    void TearDown()
    {
        /* clean up */
        destroyBox(box_ptr);
    }
};


TEST_F(testWriteFibreLine, testErrorOutput)
{
    fp = fopen(fileToWrite, "w");
    EXPECT_EQ(writeFibreLine(fp, &(box_ptr->masterFibreList[1])), 0);
    fclose(fp);
}


TEST_F(testWriteFibreLine, testWriteFibreValues)
{
    fp = fopen(fileToWrite, "w");
    writeFibreLine(fp, &(box_ptr->masterFibreList[1]));
    fclose(fp);

    Node *node0_ptr = &(box_ptr->masterNodeList[0]);
    Node *node1_ptr = &(box_ptr->masterNodeList[1]);
    Node *node2_ptr = &(box_ptr->masterNodeList[2]);

    networkRead(fileToWrite, &box_ptr, 0.05);

    EXPECT_EQ(box_ptr->masterFibreList[1].fibreID, 1);
    EXPECT_EQ(box_ptr->masterFibreList[1].nodesOnFibre, 3);
    EXPECT_EQ(box_ptr->masterFibreList[1].radius, 0.05);
    EXPECT_EQ(box_ptr->masterFibreList[1].nodesOnFibreList[0], node2_ptr);
    EXPECT_EQ(box_ptr->masterFibreList[1].nodesOnFibreList[1], node0_ptr);
    EXPECT_EQ(box_ptr->masterFibreList[1].nodesOnFibreList[2], node1_ptr);
}


struct testWriteNodeLine : ::testing::Test
{
    Box *box_ptr;
    FILE *fp;
    const char *fileToWrite;
    PetscInt *gIndex_ptr, gIndex;

    void SetUp()
    {
        fileToWrite  = "data/exWriteNodeLine.dat";

        box_ptr = makeBox(3,2,3,4,5,1,1,1);

        // additional setup
        gIndex      = 0;
        gIndex_ptr  = &gIndex;

        makeNode(box_ptr,1,2,3,4,5,gIndex_ptr,0.25);
    }

    void TearDown()
    {
        /* clean up */
        destroyBox(box_ptr);
    }
};


TEST_F(testWriteNodeLine, testErrorOutput)
{
    fp = fopen(fileToWrite, "w");
    EXPECT_EQ(writeNodeLine(fp, &(box_ptr->masterNodeList[1])), 0);
    fclose(fp);
}


TEST_F(testWriteNodeLine, testWriteNodeValues)
{
    fp = fopen(fileToWrite, "w");
    writeNodeLine(fp, &(box_ptr->masterNodeList[1]));
    fclose(fp);

    networkRead(fileToWrite, &box_ptr, 0.05);

    // test node values
    EXPECT_EQ(box_ptr->masterNodeList[1].nodeID, 1);
    EXPECT_EQ(box_ptr->masterNodeList[1].nodeType, 2);
    EXPECT_EQ(box_ptr->masterNodeList[1].xyzCoord[0], 4);   // updated with u_x=1.0 after write out
    EXPECT_EQ(box_ptr->masterNodeList[1].xyzCoord[1], 4);   // s_x + (s_y*gamma)
    EXPECT_EQ(box_ptr->masterNodeList[1].xyzCoord[2], 5);   // 3   + (4  * 0.25)
    EXPECT_EQ(box_ptr->masterNodeList[1].globalID, -1);
    EXPECT_EQ(box_ptr->masterNodeList[1].xyzDisplacement[0], 0.05 * 4);
    EXPECT_EQ(box_ptr->masterNodeList[1].xyzDisplacement[1], 0);
    EXPECT_EQ(box_ptr->masterNodeList[1].xyzDisplacement[2], 0);
}

} /* namespace */
