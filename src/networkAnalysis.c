#include "networkAnalysis.h"

/* Initiates network analysis routine */
PetscErrorCode networkAnalysis(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode 	ierr = 0;
	
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


PetscScalar calculateFibreStretchEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	PetscErrorCode  ierr = 0;
	PetscScalar 	fibEnergyStre = 0.0;
	PetscScalar 	segEnergyStre = 0.0;

    assert(DIMENSION==3);	/* re-assess whether this is still needed in the petsc model */

	Fibre *fibre_ptr = &(box_ptr->masterFibreList[fIndex]);

    /* loop through fibre in adjacent pairs */
	PetscInt i;
	for (i = 0; i < fibre_ptr->nodesOnFibre - 1; i++)
	{
		Node *alph_ptr = fibre_ptr->nodesOnFibreList[i];
		Node *beta_ptr = fibre_ptr->nodesOnFibreList[i+1];

        /* only calculate energy if neither node is dangling */
        if (alph_ptr->nodeType != NODE_DANGLING && 
        	beta_ptr->nodeType != NODE_DANGLING)
        {
            segEnergyStre = calculateSegStretchEnergy(box_ptr, par_ptr, fIndex, alph_ptr, beta_ptr);
        }

        /*
         * affine energy is calculated regardless of node types 
         */

        /* add segment energy to total for fibre */
        fibEnergyStre += segEnergyStre;
    }

	return fibEnergyStre;
}


PetscScalar calculateFibreBendEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	fibEnergyBend = 0.0;
	PetscScalar 	segEnergyBend = 0.0;

    assert(DIMENSION==3);		/* re-assess whether this is still needed in the petsc model */

	Fibre *fibre_ptr = &(box_ptr->masterFibreList[fIndex]);

    /* loop through fibre in adjacent triplets */
	PetscInt i;
	for (i = 0; i < fibre_ptr->nodesOnFibre - 2; i++)
	{
		Node *alph_ptr = fibre_ptr->nodesOnFibreList[i];
		Node *omeg_ptr = fibre_ptr->nodesOnFibreList[i+1];
		Node *beta_ptr = fibre_ptr->nodesOnFibreList[i+2];

        /* check none of the nodes are dangling */
        if (alph_ptr->nodeType != NODE_DANGLING && 
            omeg_ptr->nodeType != NODE_DANGLING &&
            beta_ptr->nodeType != NODE_DANGLING &&
            omeg_ptr->nodeType != NODE_BOUNDARY)		/* don't calculate energy for bending on boundary */
        {
            segEnergyBend = calculateSegBendEnergy(box_ptr, par_ptr, fIndex, alph_ptr, omeg_ptr, beta_ptr);
        }

        /*
         * affine energy is calculated regardless of node types 
         */

        /* add segment energy to total for fibre */
        fibEnergyBend += segEnergyBend;
    }

	return fibEnergyBend;
}


PetscErrorCode calculateEnergy(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode ierr = 0;

	PetscInt fIndex = 0;
	for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
	{
		/* calculate stretching energy from contributions of every fibre */
		par_ptr->energyStre += calculateFibreStretchEnergy(box_ptr, par_ptr, fIndex);

		/* calculate bending energy from contributions of every fibre */
		if (SPAN == 2)
		{
			par_ptr->energyBend += calculateFibreBendEnergy(box_ptr, par_ptr, fIndex);
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


PetscErrorCode calculateShearModulus(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	V;

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
    V = 1.0;
	par_ptr->shearModulus = (2 * par_ptr->energyTotl) / (V * pow(par_ptr->gamma, 2));

	return ierr;
}

