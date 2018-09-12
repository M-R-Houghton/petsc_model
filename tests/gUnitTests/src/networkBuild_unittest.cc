#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "networkBuild.h"
}


namespace {

TEST(testMakeParameters, testValueAssignment)
{
	PetscScalar gamma = 0.5, yMod = 1.0;

	Parameters *par_ptr = makeParameters(gamma,yMod);
	EXPECT_TRUE(par_ptr != NULL);

	EXPECT_EQ(par_ptr->gamma, 		  0.5);
	EXPECT_EQ(par_ptr->youngsModulus, 1.0);

	free(par_ptr); par_ptr = NULL;
	EXPECT_TRUE(par_ptr == NULL);
}


TEST(testMakeSparse, testValueAssignment)
{
	PetscInt n = 25, nz = 15;
	
	Sparse *sparse_ptr = makeSparse(n,nz);
	EXPECT_TRUE(sparse_ptr != NULL);
	EXPECT_EQ(sparse_ptr->n,  25);
	EXPECT_EQ(sparse_ptr->nz, 15);			/* test inputs */

	sparse_ptr->col[14] = 82;
	EXPECT_EQ(sparse_ptr->col[14], 82);		/* test assignment */

	/* clean up */
	free(sparse_ptr->counter); sparse_ptr->counter = NULL;
	free(sparse_ptr->rowp); sparse_ptr->rowp = NULL;
	free(sparse_ptr->col); sparse_ptr->col = NULL;
	free(sparse_ptr->mat); sparse_ptr->mat = NULL;
	free(sparse_ptr); sparse_ptr = NULL;
	EXPECT_TRUE(sparse_ptr == NULL);
}


TEST(testMakeBox, testValueAssignment)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	EXPECT_TRUE(box_ptr != NULL);
	EXPECT_EQ(box_ptr->nodeCount,  			1);
	EXPECT_EQ(box_ptr->fibreCount,  		2);
	EXPECT_EQ(box_ptr->nodeInternalCount,  -1);
	EXPECT_EQ(box_ptr->xyzDimension[0],  	3);
	EXPECT_EQ(box_ptr->xyzDimension[1],  	4);
	EXPECT_EQ(box_ptr->xyzDimension[2],  	5);
	EXPECT_EQ(box_ptr->xyzPeriodic[0],  	1);
	EXPECT_EQ(box_ptr->xyzPeriodic[1],  	1);
	EXPECT_EQ(box_ptr->xyzPeriodic[2],  	1);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    EXPECT_TRUE(box_ptr == NULL);
}


TEST(testMakeBox, testNodeListInitialisation)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	EXPECT_TRUE(box_ptr->masterNodeList != NULL);
	EXPECT_EQ(box_ptr->masterNodeList[0].nodeID, 				0);
	EXPECT_EQ(box_ptr->masterNodeList[0].nodeID, 				0);
	EXPECT_EQ(box_ptr->masterNodeList[0].nodeType, 				0);
	EXPECT_EQ(box_ptr->masterNodeList[0].globalID, 				0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzCoord[0], 			0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzCoord[1], 			0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzCoord[2], 			0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzDisplacement[0],	0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzDisplacement[1],	0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzDisplacement[2],	0);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    EXPECT_TRUE(box_ptr == NULL);
}


TEST(testMakeBox, testFibreListInitialisation)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	EXPECT_TRUE(box_ptr->masterFibreList != NULL);

	EXPECT_EQ(box_ptr->masterFibreList[0].fibreID, 		0);
	EXPECT_EQ(box_ptr->masterFibreList[0].nodesOnFibre, 0);
	EXPECT_EQ(box_ptr->masterFibreList[0].radius, 		0);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    EXPECT_TRUE(box_ptr == NULL);
}


TEST(testMakeFibre, testValueAssignment)
{
	/* set up test */
	Box *box_ptr = makeBox(3,2,3,4,5,1,1,1);
	Node *node1_ptr = &(box_ptr->masterNodeList[0]);
	Node *node2_ptr = &(box_ptr->masterNodeList[1]);

	/* set up node list and make fibre */
	Node **node_ptr_list = (Node**)calloc(1, sizeof(Node*));

	node_ptr_list[0] = &(box_ptr->masterNodeList[0]);
	makeFibre(box_ptr,0,1,0.01,node_ptr_list);

	/* test fibre values */
	EXPECT_EQ(box_ptr->masterFibreList[0].fibreID, 				0);
	EXPECT_EQ(box_ptr->masterFibreList[0].nodesOnFibre, 		1);
	EXPECT_EQ(box_ptr->masterFibreList[0].radius, 				0.01);
	EXPECT_EQ(box_ptr->masterFibreList[0].nodesOnFibreList[0], 	node1_ptr);
	EXPECT_NE(box_ptr->masterFibreList[0].nodesOnFibreList[0], 	node2_ptr);

	/* change node in node list */
	node_ptr_list[0] = &(box_ptr->masterNodeList[2]);
	makeFibre(box_ptr,1,1,0.05,node_ptr_list);

	/* test fibre values */
	EXPECT_EQ(box_ptr->masterFibreList[1].fibreID, 1);
	EXPECT_EQ(box_ptr->masterFibreList[1].nodesOnFibre, 1);
	EXPECT_EQ(box_ptr->masterFibreList[1].radius, 0.05);
	EXPECT_NE(box_ptr->masterFibreList[1].nodesOnFibreList[0], node1_ptr);
	EXPECT_NE(box_ptr->masterFibreList[1].nodesOnFibreList[0], node2_ptr);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	EXPECT_TRUE(box_ptr->masterNodeList == NULL);
    free(node_ptr_list); node_ptr_list = NULL;
    EXPECT_TRUE(node_ptr_list == NULL);
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
	EXPECT_TRUE(box_ptr->masterFibreList == NULL);
    free(box_ptr); box_ptr = NULL;
    EXPECT_TRUE(box_ptr == NULL);
}


TEST(testMakeNode, testValueAssignment)
{
	/* set up test */
	Box *box_ptr = makeBox(3,2,3,4,5,1,1,1);
	PetscInt myInt = 1;
	PetscInt *myInt_ptr = &myInt;

	/* make some nodes */
	makeNode(box_ptr,1,2,3,4,5,myInt_ptr,0.5);
	makeNode(box_ptr,0,1,2,3,4,myInt_ptr,0.25);

	/* test node values */
	EXPECT_EQ(box_ptr->masterNodeList[1].nodeID, 1);
	EXPECT_EQ(box_ptr->masterNodeList[1].nodeType, 2);
	EXPECT_EQ(box_ptr->masterNodeList[1].xyzCoord[0], 3);
	EXPECT_EQ(box_ptr->masterNodeList[1].xyzCoord[1], 4);
	EXPECT_EQ(box_ptr->masterNodeList[1].xyzCoord[2], 5);
	EXPECT_EQ(box_ptr->masterNodeList[1].globalID, -1);
	EXPECT_EQ(box_ptr->masterNodeList[1].xyzDisplacement[0], 0.5 * 4);
	EXPECT_EQ(box_ptr->masterNodeList[1].xyzDisplacement[1], 0);
	EXPECT_EQ(box_ptr->masterNodeList[1].xyzDisplacement[2], 0);

	/* test node values */
	EXPECT_EQ(box_ptr->masterNodeList[0].nodeID, 0);
	EXPECT_EQ(box_ptr->masterNodeList[0].nodeType, 1);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzCoord[0], 2);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzCoord[1], 3);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzCoord[2], 4);
	EXPECT_EQ(box_ptr->masterNodeList[0].globalID, -1);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzDisplacement[0], 0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzDisplacement[1], 0);
	EXPECT_EQ(box_ptr->masterNodeList[0].xyzDisplacement[2], 0);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    EXPECT_TRUE(box_ptr == NULL);
}

} /* namespace */
