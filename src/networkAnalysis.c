#include "networkAnalysis.h"

/* Initiates network analysis routine */
PetscErrorCode networkAnalysis()
{
	PetscErrorCode 	ierr;
	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Analysing network...\n");CHKERRQ(ierr);
	
	return ierr;
}


PetscScalar calculateSegStretchEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
										Node *alph_ptr, Node *beta_ptr )
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscScalar calculateSegBendEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
									 Node *alph_ptr, Node *omeg_ptr, Node *beta_ptr )
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscScalar calculateStretchEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscScalar calculateBendEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscErrorCode calculateEnergy(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode ierr = 0;

	PetscInt fIndex = 0;
	for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
	{
		/* calculate stretching energy from contributions of every fibre */
		par_ptr->energyStre += calculateStretchEnergy(box_ptr, par_ptr, fIndex);

		/* calculate bending energy from contributions of every fibre */
		if (SPAN == 2)
		{
			par_ptr->energyBend += calculateBendEnergy(box_ptr, par_ptr, fIndex);
		}
	}

	if (SPAN != 2) assert(par_ptr->energyBend == 0.0);

	/* calculate total energy after looping over every fibre */
	par_ptr->energyTotl = par_ptr->energyStre + par_ptr->energyBend;

	return ierr;
}


PetscScalar calculateArea(Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscScalar calculateVolume(Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscScalar aperiodicRange(Box *box_ptr, PetscInt i)
{
	PetscErrorCode ierr = 0;

	return 0.0;
}


PetscScalar calculateShearModulus(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	V,G;

	calculateEnergy(box_ptr, par_ptr);CHKERRQ(ierr);

	/* currently need to use separate calculations for area and volume */
	if (DIMENSION == 2)
	{
		V = calculateArea(box_ptr);
	}
	else if (DIMENSION == 3)
	{
		V = calculateVolume(box_ptr);
	}

    /* use energy and volume/area to calculate the shear modulus */
	G = (2 * par_ptr->energyTotl) / (V * pow(par_ptr->gamma, 2));

	return G;
}

