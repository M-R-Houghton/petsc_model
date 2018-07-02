#include <criterion/criterion.h>
#include "vectorOperations.h"

Test(testMinInt, testOutputValues)
{
	cr_expect(minInt( 1, 2) ==  1);
	cr_expect(minInt( 4, 4) ==  4);
	cr_expect(minInt( 7, 3) ==  3);
	cr_expect(minInt( 1,-2) == -2);
	cr_expect(minInt(-4, 4) == -4);
	cr_expect(minInt( 7,-3) == -3);
}


Test(testMaxInt, testOutputValues)
{
	cr_expect(maxInt( 1, 2) ==  2);
	cr_expect(maxInt( 4, 4) ==  4);
	cr_expect(maxInt( 7, 3) ==  7);
	cr_expect(maxInt( 1,-2) ==  1);
	cr_expect(maxInt(-4, 4) ==  4);
	cr_expect(maxInt( 7,-3) ==  7);
	cr_expect(maxInt(-4,-3) == -3);
}


Test(testMinScalar, testOutputValues)
{
	cr_expect(minScalar( 1.0, 2.0) ==  1.0);
	cr_expect(minScalar( 4.0, 4.0) ==  4.0);
	cr_expect(minScalar( 7.0, 3.0) ==  3.0);
	cr_expect(minScalar( 1.0,-2.0) == -2.0);
	cr_expect(minScalar(-4.0, 4.0) == -4.0);
	cr_expect(minScalar( 7.0,-3.0) == -3.0);
}


Test(testMaxScalar, testOutputValues)
{
	cr_expect(maxScalar( 1.8, 2.2) ==  2.2);
	cr_expect(maxScalar( 4.2, 4.2) ==  4.2);
	cr_expect(maxScalar( 7.8, 3.2) ==  7.8);
	cr_expect(maxScalar( 1.8,-2.2) ==  1.8);
	cr_expect(maxScalar(-4.8, 4.2) ==  4.2);
	cr_expect(maxScalar( 7.8,-3.2) ==  7.8);
	cr_expect(maxScalar(-4.8,-3.2) == -3.2);
}


Test(testVecDotProduct, testOutputValues)
{
	PetscScalar vec1[2] = {1.0, 2.0};
	PetscScalar vec2[2] = {3.0, 4.0};
	PetscScalar vec3[3] = {1.0, 2.0, 3.0};
	PetscScalar vec4[3] = {4.0, 5.0, 6.0};

	cr_expect(vecDotProduct(vec1, vec2) == 0);
	cr_expect(vecDotProduct(vec3, vec4) == 0);
}


Test(testVec2DCrossProduct, testOutputValues)
{
	PetscScalar vec1[2] = {1.0, 2.0};
	PetscScalar vec2[2] = {3.0, 4.0};

	cr_expect(vec2DCrossProduct(vec1, vec2) == 0);
}


Test(testVec3DCrossProduct, testOutputValues)
{
	PetscScalar vec1[3];
	PetscScalar vec2[3] = {1.0, 2.0, 3.0};
	PetscScalar vec3[3] = {4.0, 5.0, 6.0};

	cr_expect(vec3DCrossProduct(vec1, vec2, vec3) == 0);
}


Test(testVecMagnitude, testOutputValues)
{
	PetscScalar vec1[2] = {1.0, 2.0};
	PetscScalar vec2[3] = {1.0, 2.0, 3.0};

	cr_expect(vecMagnitude(vec1) == 0);
	cr_expect(vecMagnitude(vec2) == 0);
}


Test(testVecAddition, testOutputValues)
{
	Box *box_ptr = (Box *)malloc(sizeof(Box));

	PetscScalar vec1[2];
	PetscScalar vec2[2] = {3.0, 4.0};
	PetscScalar vec3[2] = {1.0, 1.0};

	PetscScalar vec4[3];
	PetscScalar vec5[3] = {1.0, 2.0, 3.0};
	PetscScalar vec6[3] = {4.0, 5.0, 6.0};

	cr_expect(vecAddition(vec1, vec2, vec3, box_ptr) == 0);
	cr_expect(vecAddition(vec4, vec5, vec6, box_ptr) == 0);
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicX)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 0) == 0);
	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 0) == 0);

	cr_expect_float_eq(vec1[0], 0.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	cr_expect_float_eq(vec2[0], 0.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicX)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 0) == 0);
	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 0) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicY)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 1) == 0);
	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 1) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 0.0, tol);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 0.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicY)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 1) == 0);
	cr_expect(nearestSegmentCopyDirN(vec2, box_ptr, 1) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForPeriodicZ)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 1;
	PetscScalar vec1[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 2) == 0);

	cr_expect_float_eq(vec1[0], 1.0, tol);
	cr_expect_float_eq(vec1[1], 2.0, tol);
	cr_expect_float_eq(vec1[2], 0.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopyDirN, testOutputValuesForAperiodicZ)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopyDirN(vec1, box_ptr, 2) == 0);

	cr_expect_float_eq(vec1[0], 1.0, tol);
	cr_expect_float_eq(vec1[1], 2.0, tol);
	cr_expect_float_eq(vec1[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopy, testOutputValuesForPeriodic)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 1;
	box_ptr->xyzPeriodic[1] 	= 1;
	box_ptr->xyzPeriodic[2] 	= 1;
	PetscScalar vec1[2] 		= {2.0, 3.0};
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopy(vec1, box_ptr) == 0);
	cr_expect(nearestSegmentCopy(vec2, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], 0.0, tol);
	cr_expect_float_eq(vec1[1], 0.0, tol);

	cr_expect_float_eq(vec2[0], 0.0, tol);
	cr_expect_float_eq(vec2[1], 0.0, tol);
	cr_expect_float_eq(vec2[2], 0.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}


Test(testNearestSegmentCopy, testOutputValuesForAPeriodic)
{
	PetscScalar tol 			= 1e-12;
	Box *box_ptr 				= (Box *)malloc(sizeof(Box));
	box_ptr->xyzDimension[0] 	= 1;
	box_ptr->xyzDimension[1] 	= 1;
	box_ptr->xyzDimension[2] 	= 1;
	box_ptr->xyzPeriodic[0] 	= 0;
	box_ptr->xyzPeriodic[1] 	= 0;
	box_ptr->xyzPeriodic[2] 	= 0;
	PetscScalar vec1[2] 		= {2.0, 3.0};
	PetscScalar vec2[3] 		= {1.0, 2.0, 3.0};

	cr_expect(nearestSegmentCopy(vec1, box_ptr) == 0);
	cr_expect(nearestSegmentCopy(vec2, box_ptr) == 0);

	cr_expect_float_eq(vec1[0], 2.0, tol);
	cr_expect_float_eq(vec1[1], 3.0, tol);

	cr_expect_float_eq(vec2[0], 1.0, tol);
	cr_expect_float_eq(vec2[1], 2.0, tol);
	cr_expect_float_eq(vec2[2], 3.0, tol);

	/* clean up */
    free(box_ptr); box_ptr = NULL;
}

