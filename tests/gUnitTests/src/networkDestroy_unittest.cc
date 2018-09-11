#include <petscksp.h>
#include "gtest/gtest.h"

/* include c source file */
extern "C" 
{
    #include "networkDestroy.h"
}


namespace {

TEST(testDestroyParameters, testErrorOutput)
{
	Parameters *par_ptr = makeParameters(0.5,0.1);
	EXPECT_EQ(destroyParameters(par_ptr), 0);

	Parameters *other_par_ptr = makeParameters(2.7,0.3);
	EXPECT_EQ(destroyParameters(other_par_ptr), 0);
}


TEST(testDestroyParameters, testRemovedValues)
{
	Parameters *par_ptr = makeParameters(0.5,0.1);
	destroyParameters(par_ptr);

	Parameters *other_par_ptr = makeParameters(2.7,0.3);
	destroyParameters(other_par_ptr);

	EXPECT_NE(par_ptr->gamma, 		  0.5);
	EXPECT_NE(par_ptr->youngsModulus, 1.0);
}


TEST(testDestroySparse, testErrorOutput)
{	
	Sparse *sparse_ptr = makeSparse(25,15);
	EXPECT_EQ(destroySparse(sparse_ptr), 0);

	// Overwrite memory with something else
	Sparse *other_sparse_ptr = makeSparse(10,8);
	EXPECT_EQ(destroySparse(other_sparse_ptr), 0);
}


TEST(testDestroySparse, testRemovedValues)
{	
	Sparse *sparse_ptr = makeSparse(25,15);
	destroySparse(sparse_ptr);

	// Overwrite memory with something else
	Sparse *other_sparse_ptr = makeSparse(10,8);
	destroySparse(other_sparse_ptr);

	EXPECT_NE(sparse_ptr->n,  25);
	EXPECT_NE(sparse_ptr->nz, 15);
}


TEST(testDestroyBox, testErrorOutput)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	EXPECT_EQ(destroyBox(box_ptr), 0);
}


TEST(testDestroyBox, testRemovedValues)
{
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	destroyBox(box_ptr);

	int   someInt   = 3987;
	float someFloat = -70521.99328765;
	float someCalculation;

	someCalculation = someInt + someFloat;

	Box *other_box_ptr = makeBox(5,7,9,3,1,0,0,0);
	other_box_ptr->nodeInternalCount = 56;
	destroyBox(other_box_ptr);

	EXPECT_NE(box_ptr->nodeCount,  			1);
	EXPECT_NE(box_ptr->fibreCount,  		2);
	EXPECT_NE(box_ptr->nodeInternalCount,  -1);
	EXPECT_NE(box_ptr->xyzDimension[0],  	3);
	EXPECT_NE(box_ptr->xyzDimension[1],  	4);
	EXPECT_NE(box_ptr->xyzDimension[2],  	5);
	EXPECT_NE(box_ptr->xyzPeriodic[0],  	1);
	EXPECT_NE(box_ptr->xyzPeriodic[1],  	1);
	EXPECT_NE(box_ptr->xyzPeriodic[2],  	1);
}

} /* namespace */
