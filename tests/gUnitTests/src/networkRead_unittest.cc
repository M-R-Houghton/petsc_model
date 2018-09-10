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
    //Box **box_ptr;
    FILE *fp;
    const char *fileName;
    char line[100], *line_ptr;
    void SetUp()
    {
        fileName                    = "data/exReadBoxLine.dat";
        //box_ptr                     = (Box *)malloc(sizeof(Box));
        //box_ptr->xyzDimension[0]    = 0;
        //box_ptr->xyzDimension[1]    = 0;
        //box_ptr->xyzDimension[2]    = 0;
        //box_ptr->xyzPeriodic[0]     = 0;
        //box_ptr->xyzPeriodic[1]     = 0;
        //box_ptr->xyzPeriodic[2]     = 0;
    }

    void TearDown()
    {
        /* clean up */
        //free(box_ptr); box_ptr = NULL;
    }
};


TEST_F(testReadBoxLine, testErrorOutput)
{
    Box *box_ptr;
    fp = fopen(fileName, "r");

    line_ptr = fgets(line, sizeof(line), fp);
    EXPECT_EQ(readBoxLine(line_ptr, &box_ptr), 0);

    free(box_ptr->masterNodeList);
    box_ptr->masterNodeList = NULL;

    // free node list of each fibre
    int f;
    for (f = 0; f < box_ptr->fibreCount; f++)
    {
        free(box_ptr->masterFibreList[f].nodesOnFibreList); 
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
    }

    // free the master list of fibres
    free(box_ptr->masterFibreList);
    box_ptr->masterFibreList = NULL;
     
    // free the rest of the box
    free(box_ptr);
    box_ptr = NULL;

    fclose(fp);
}


TEST_F(testReadBoxLine, testReadValues)
{
    ASSERT_TRUE(DIMENSION == 3);

    Box *box_ptr;
    fp = fopen(fileName, "r");
    
    line_ptr = fgets(line, sizeof(line), fp);
    readBoxLine(line_ptr, &box_ptr);

    EXPECT_EQ(box_ptr->xyzDimension[0], 1);
    EXPECT_EQ(box_ptr->xyzDimension[1], 2);
    EXPECT_EQ(box_ptr->xyzDimension[2], 3);
    EXPECT_EQ(box_ptr->xyzPeriodic[0],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[1],  1);
    EXPECT_EQ(box_ptr->xyzPeriodic[2],  1);

    free(box_ptr->masterNodeList);
    box_ptr->masterNodeList = NULL;

    // free node list of each fibre
    int f;
    for (f = 0; f < box_ptr->fibreCount; f++)
    {
        free(box_ptr->masterFibreList[f].nodesOnFibreList); 
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
    }

    // free the master list of fibres
    free(box_ptr->masterFibreList);
    box_ptr->masterFibreList = NULL;
     
    // free the rest of the box
    free(box_ptr);
    box_ptr = NULL;
    
    fclose(fp);
}

} /* namespace */
