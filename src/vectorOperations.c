#include "vectorOperations.h"

/* Calculates the minimum of two integers */
PetscInt minInt(PetscInt a, PetscInt b)
{
	if (a < b) 
	{
		return a;
	}
	return b;
}


/* Calculates the maximum of two integers */
PetscInt maxInt(PetscInt a, PetscInt b)
{
	if (a > b) 
	{
		return a;
	}
	return b;
}


/* Calculates the minimum of two scalars */
PetscScalar minScalar(PetscScalar a, PetscScalar b)
{
	if (a < b) 
	{
		return a;
	}
	return b;
}


/* Calculates the maximum of two scalars */
PetscScalar maxScalar(PetscScalar a, PetscScalar b)
{
	if (a > b) 
	{
		return a;
	}
	return b;
}


/* Calculates the dot product of two given vectors */
PetscScalar vecDotProduct(PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	return 0;
}


/* Calculates the cross product of two given 2D vectors */
PetscScalar vec2DCrossProduct(PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	return 0;
}


/* Calculates the cross product of two given 3D vectors */
PetscErrorCode vec3DCrossProduct(PetscScalar *crossVec_ptr, PetscScalar *vec1_ptr, PetscScalar *vec2_ptr)
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Calculates the magnitude of a given vector */
PetscScalar vecMagnitude(PetscScalar *vec_ptr)
{
	return 0;
}


/* Calculates the addition of two position vectors */
PetscErrorCode vecAddition(PetscScalar *addVec_ptr, PetscScalar *posVec1_ptr, PetscScalar *posVec2_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Creates the unit tangent vector of a given vector */
PetscErrorCode makeTangentVec(PetscScalar *tangVec_ptr, PetscScalar *vec_ptr)
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Creates the distance vector between two position vectors */
PetscErrorCode makeDistanceVec(PetscScalar *distVec_ptr, PetscScalar *posVec1_ptr, PetscScalar *posVec2_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Checks whether a segment crosses the N boundary and updates it to the nearest copy inside the domain */
PetscErrorCode nearestSegmentCopyDirN(PetscScalar *distVec_ptr, Box *box_ptr, PetscInt N)
{
	PetscErrorCode ierr = 0;

	if (box_ptr->xyzPeriodic[N] == 1)
	{
		PetscScalar dimN = box_ptr->xyzDimension[N]; 	/* i.e. width/height/depth */

		if (distVec_ptr[N] > 0.5 * dimN) 
		{
			distVec_ptr[N] -= dimN;
		} 
		else if (distVec_ptr[N] < -0.5 * dimN) 
		{
			distVec_ptr[N] += dimN;
		}
	}
	return ierr;
}


/* Checks whether a segment crosses any boundary and updates it to the nearest copy inside the domain */
PetscErrorCode nearestSegmentCopy(PetscScalar *distVec_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	/* check if x-periodic */
	ierr = nearestSegmentCopyDirN(distVec_ptr, box_ptr, 0);CHKERRQ(ierr);

	/* check if y-periodic */
	ierr = nearestSegmentCopyDirN(distVec_ptr, box_ptr, 1);CHKERRQ(ierr);

	/* check if z-periodic */
	ierr = nearestSegmentCopyDirN(distVec_ptr, box_ptr, 2);CHKERRQ(ierr);

	return ierr;
}


