#include <petscksp.h>
#include <criterion/criterion.h>
#include "networkBuild.h"

Test(testMakeParameters, testValueAssignment)
{
	PetscScalar gamma = 0.5, yMod = 1.0;

	Parameters *par_ptr = makeParameters(gamma,yMod);
	cr_expect_not_null(par_ptr);

	cr_expect(par_ptr->gamma         == 0.5);
	cr_expect(par_ptr->youngsModulus == 1.0);

	free(par_ptr); par_ptr = NULL;
	cr_expect_null(par_ptr);
}


Test(testMakeSparse, testValueAssignment)
{
	PetscInt n = 25, nz = 15;
	
	Sparse *sparse_ptr = makeSparse(n,nz);
	cr_expect_not_null(sparse_ptr);
	cr_expect(sparse_ptr->n  == 25);
	cr_expect(sparse_ptr->nz == 15);			/* test inputs */

	sparse_ptr->col[14] = 82;
	cr_expect(sparse_ptr->col[14] == 82);		/* test assignment */

	/* clean up */
	free(sparse_ptr->counter); sparse_ptr->counter = NULL;
	free(sparse_ptr->rowp); sparse_ptr->rowp = NULL;
	free(sparse_ptr->col); sparse_ptr->col = NULL;
	free(sparse_ptr->mat); sparse_ptr->mat = NULL;
	free(sparse_ptr); sparse_ptr = NULL;
	cr_expect_null(sparse_ptr);
}


Test(testCheckBoxArguments, testValidArguments)
{
	cr_expect(checkBoxArguments(1,2,3,4,5,0,0,0) == 0);
	cr_expect(checkBoxArguments(1,2,3,4,5,1,1,1) == 0);
}


Test(testMakeBox, testValueAssignment)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	cr_expect_not_null(box_ptr);
	cr_expect(box_ptr->nodeCount 		 ==  1);
	cr_expect(box_ptr->fibreCount 		 ==  2);
	cr_expect(box_ptr->nodeInternalCount == -1);
	cr_expect(box_ptr->xyzDimension[0] 	 ==  3);
	cr_expect(box_ptr->xyzDimension[1] 	 ==  4);
	cr_expect(box_ptr->xyzDimension[2] 	 ==  5);
	cr_expect(box_ptr->xyzPeriodic[0] 	 ==  1);
	cr_expect(box_ptr->xyzPeriodic[1] 	 ==  1);
	cr_expect(box_ptr->xyzPeriodic[2] 	 ==  1);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    cr_expect_null(box_ptr);
}


Test(testMakeBox, testNodeListInitialisation)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	cr_expect_not_null(box_ptr->masterNodeList);
	cr_expect(box_ptr->masterNodeList[0].nodeID 			== 0);
	cr_expect(box_ptr->masterNodeList[0].nodeID 			== 0);
	cr_expect(box_ptr->masterNodeList[0].nodeType 			== 0);
	cr_expect(box_ptr->masterNodeList[0].globalID 			== 0);
	cr_expect(box_ptr->masterNodeList[0].xyzCoord[0] 		== 0);
	cr_expect(box_ptr->masterNodeList[0].xyzCoord[1] 		== 0);
	cr_expect(box_ptr->masterNodeList[0].xyzCoord[2] 		== 0);
	cr_expect(box_ptr->masterNodeList[0].xyzDisplacement[0] == 0);
	cr_expect(box_ptr->masterNodeList[0].xyzDisplacement[1] == 0);
	cr_expect(box_ptr->masterNodeList[0].xyzDisplacement[2] == 0);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    cr_expect_null(box_ptr);
}


Test(testMakeBox, testFibreListInitialisation)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	cr_expect_not_null(box_ptr->masterFibreList);

	cr_expect(box_ptr->masterFibreList[0].fibreID 			== 0);
	cr_expect(box_ptr->masterFibreList[0].nodesOnFibre 		== 0);
	cr_expect(box_ptr->masterFibreList[0].radius 			== 0);
    cr_expect(box_ptr->masterFibreList[0].nodesOnFibreList 	== 0);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	for (int f = 0; f < box_ptr->fibreCount; f++)
	{
        free(box_ptr->masterFibreList[f].nodesOnFibreList);	
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
	}
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
    free(box_ptr); box_ptr = NULL;
    cr_expect_null(box_ptr);
}


Test(testMakeFibre, testValueAssignment)
{
	/* set up test */
	Box *box_ptr = makeBox(3,2,3,4,5,1,1,1);
	Node *node1_ptr = &(box_ptr->masterNodeList[0]);
	Node *node2_ptr = &(box_ptr->masterNodeList[1]);

	/* set up node list and make fibre */
	Node **node_ptr_list = (Node**)calloc(1, sizeof(Node*));

	node_ptr_list[0] = &(box_ptr->masterNodeList[0]);
	makeFibre(box_ptr, 0, 1, 0.01, node_ptr_list);

	/* test fibre values */
	cr_expect(box_ptr->masterFibreList[0].fibreID 		== 0);
	cr_expect(box_ptr->masterFibreList[0].nodesOnFibre 	== 1);
	cr_expect(box_ptr->masterFibreList[0].radius 		== 0.01);
	cr_expect(box_ptr->masterFibreList[0].nodesOnFibreList[0] 	  == node1_ptr);
	cr_expect_not(box_ptr->masterFibreList[0].nodesOnFibreList[0] == node2_ptr);

	/* change node in node list */
	node_ptr_list[0] = &(box_ptr->masterNodeList[2]);
	makeFibre(box_ptr, 1, 1, 0.05, node_ptr_list);

	/* test fibre values */
	cr_expect(box_ptr->masterFibreList[1].fibreID 		== 1);
	cr_expect(box_ptr->masterFibreList[1].nodesOnFibre 	== 1);
	cr_expect(box_ptr->masterFibreList[1].radius 		== 0.05);
	cr_expect_not(box_ptr->masterFibreList[1].nodesOnFibreList[0] == node1_ptr);
	cr_expect_not(box_ptr->masterFibreList[1].nodesOnFibreList[0] == node2_ptr);

	/* clean up */
	free(box_ptr->masterNodeList); box_ptr->masterNodeList = NULL;
	cr_expect_null(box_ptr->masterNodeList);
    free(node_ptr_list); node_ptr_list = NULL;
    cr_expect_null(node_ptr_list);
	free(box_ptr->masterFibreList); box_ptr->masterFibreList = NULL;
	cr_expect_null(box_ptr->masterFibreList);
    free(box_ptr); box_ptr = NULL;
    cr_expect_null(box_ptr);
}


Test(testMakeNode, testValueAssignment)
{
	//cr_expect_null(makeNode(1,1,1,1,1,1,1,1));
}



