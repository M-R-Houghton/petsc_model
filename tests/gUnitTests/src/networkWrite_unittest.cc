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
    void SetUp()
    {
        fileToWrite  = "data/exWriteFibreLine.dat";

        box_ptr = makeBox(3,2,3,4,5,1,1,1);

        /* set up node list and make fibre */
        Node **node_ptr_list = (Node**)calloc(3, sizeof(Node*));

        /* change node in node list */
        node_ptr_list[0] = &(box_ptr->masterNodeList[2]);
        node_ptr_list[1] = &(box_ptr->masterNodeList[0]);
        node_ptr_list[2] = &(box_ptr->masterNodeList[1]);

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
    EXPECT_EQ(writeFibreLine(fp, &(box_ptr->masterFibreList[1]), 1), 0);
    fclose(fp);
}


TEST_F(testWriteFibreLine, testWriteBoxValues)
{
    fp = fopen(fileToWrite, "w");
    writeFibreLine(fp, &(box_ptr->masterFibreList[1]), 1);
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

} /* namespace */
