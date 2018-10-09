#include "networkAnalysis.h"

/* Initiates network analysis routine */
PetscErrorCode networkAnalysis(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode ierr;

	ierr = calculateShearModulus(box_ptr, par_ptr);CHKERRQ(ierr);
	
	/* 
     * can add more measures of analysis into this function as needed 
     */
	
	return ierr;
}


PetscScalar calculateSegStretchEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
										PetscScalar *s_alph, PetscScalar *s_beta,
                                        PetscScalar *u_alph, PetscScalar *u_beta )
{
	PetscErrorCode 	ierr = 0;
	PetscScalar		l_alphBeta, k;
	PetscScalar 	u_dot_t, segStreEnergy;

	/* setup static vectors */
    PetscScalar 	s_alphBeta[DIMENSION];
    PetscScalar 	t_alphBeta[DIMENSION];
    PetscScalar 	u_alphBeta[DIMENSION];

    /* make distance vector between alpha and beta */
    ierr = makeDistanceVec(s_alphBeta, s_alph, s_beta, box_ptr);CHKERRQ(ierr);

    /* make tangent vector of segment */
    ierr = makeTangentVec(t_alphBeta, s_alphBeta);CHKERRQ(ierr);

    /* make distance vector between alpha and beta displacements */
    ierr = makeDistanceVec(u_alphBeta, u_alph, u_beta, box_ptr);CHKERRQ(ierr);

    /* find length of segment */
    l_alphBeta = vecMagnitude(s_alphBeta);

    /* calculate stretching term k */
    k = calculateK(box_ptr, par_ptr, fIndex, l_alphBeta);

    u_dot_t = vecDotProduct(u_alphBeta, t_alphBeta);

    segStreEnergy = (k / 2) * pow(u_dot_t, 2);

	return segStreEnergy;
}


PetscScalar calculateSegBendEnergy( Box *box_ptr, Parameters *par_ptr, PetscInt fIndex,
									 PetscScalar *s_alph, PetscScalar *s_omeg, PetscScalar *s_beta,
                                     PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta )
{
	PetscErrorCode 	ierr = 0;
	PetscScalar		kappa;
	PetscScalar		l_alphOmeg, l_omegBeta, l_alphBeta;
	PetscScalar		bConstNum, bConstDen, bConst;
	PetscScalar 	phiMagnitude, segBendEnergy;

	/* declare static vectors */
    PetscScalar s_alphOmeg[DIMENSION];
    PetscScalar s_omegBeta[DIMENSION];
    PetscScalar u_alphOmeg[DIMENSION];
    PetscScalar u_omegBeta[DIMENSION];

    /* declare static cross product vectors */
    PetscScalar s_cross_u[DIMENSION];
    PetscScalar u_cross_s[DIMENSION];

    /* declare vector to store sum of cross products */
    PetscScalar phi[DIMENSION];

    /* make distance vectors */
    ierr = makeDistanceVec(s_alphOmeg, s_alph, s_omeg, box_ptr);CHKERRQ(ierr);
    ierr = makeDistanceVec(s_omegBeta, s_omeg, s_beta, box_ptr);CHKERRQ(ierr);

    /* calculate segment lengths */
	l_alphOmeg = vecMagnitude(s_alphOmeg);
	l_omegBeta = vecMagnitude(s_omegBeta);	/* WARNING: do NOT assume that 			*/
	l_alphBeta = l_alphOmeg + l_omegBeta;	/* l_alphBeta = vecMagnitude(s_alpBeta) */
    
    /* calculate bending modulus kappa */
    kappa = calculateKappa(box_ptr, par_ptr, fIndex);

	/* calculate bending constant */
	bConstNum = 2 * kappa;
	bConstDen = l_alphBeta * pow(l_alphOmeg,2) * pow(l_omegBeta,2);
	bConst 	  = bConstNum / bConstDen;

    /* make distance vectors */
    ierr = makeDistanceVec(u_alphOmeg, u_alph, u_omeg, box_ptr);CHKERRQ(ierr);
    ierr = makeDistanceVec(u_omegBeta, u_omeg, u_beta, box_ptr);CHKERRQ(ierr);

    /* cross s_alphaOmega with u_omegaBeta, and u_alphaOmega with s_omegaBeta */
    vec3DCrossProduct(s_cross_u, s_alphOmeg, u_omegBeta);
    vec3DCrossProduct(u_cross_s, u_alphOmeg, s_omegBeta);

    /* add the two crosses together and calculate the magnitude to get phi magnitude */
    vecAddition(phi, s_cross_u, u_cross_s, box_ptr);
    phiMagnitude = vecMagnitude(phi);

    segBendEnergy = bConst * pow(phiMagnitude, 2);

	return segBendEnergy;
}


PetscErrorCode calculateFibreStretchEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	PetscErrorCode  ierr = 0;
	PetscScalar 	segStreEnergy = 0.0;
    PetscScalar     segAffnEnergy = 0.0;

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
            /* add segment energy to total for fibre */
            segStreEnergy = calculateSegStretchEnergy( box_ptr, par_ptr, fIndex, alph_ptr->xyzCoord, beta_ptr->xyzCoord, 
                                                        alph_ptr->xyzDisplacement, beta_ptr->xyzDisplacement );
            fibre_ptr->fibreStreEnergy += segStreEnergy;
        }

        /*
         * affine energy is calculated regardless of node types 
         * NOTE: should be passing affine displacements to calculateSegStretchEnergy()
         */
        segAffnEnergy = calculateSegStretchEnergy( box_ptr, par_ptr, fIndex, alph_ptr->xyzCoord, beta_ptr->xyzCoord,
                                                   alph_ptr->xyzAffDisplacement, beta_ptr->xyzAffDisplacement );
        fibre_ptr->fibreAffnEnergy += segAffnEnergy;
    }

	return ierr;
}


PetscErrorCode calculateFibreBendEnergy(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	segBendEnergy = 0.0;

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
            /* add segment energy to total for fibre */
            segBendEnergy = calculateSegBendEnergy( box_ptr, par_ptr, fIndex, alph_ptr->xyzCoord, omeg_ptr->xyzCoord, beta_ptr->xyzCoord,
                                                    alph_ptr->xyzDisplacement, omeg_ptr->xyzDisplacement, beta_ptr->xyzDisplacement );
            fibre_ptr->fibreBendEnergy += segBendEnergy;
        }
        /* NOTE: affine energy is not calculated for bending */
    }

	return ierr;
}


PetscErrorCode calculateEnergy(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode ierr = 0;

	PetscInt fIndex = 0;
	for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
	{
		/* calculate stretching energy from contributions of every fibre */
		ierr = calculateFibreStretchEnergy(box_ptr, par_ptr, fIndex);CHKERRQ(ierr);
        par_ptr->energyStre += box_ptr->masterFibreList[fIndex].fibreStreEnergy;
        par_ptr->energyAffn += box_ptr->masterFibreList[fIndex].fibreAffnEnergy;

		/* calculate bending energy from contributions of every fibre */
		if (SPAN == 2)
		{
			ierr = calculateFibreBendEnergy(box_ptr, par_ptr, fIndex);CHKERRQ(ierr);
            par_ptr->energyBend += box_ptr->masterFibreList[fIndex].fibreBendEnergy;
		}
	}

	if (SPAN != 2) assert(par_ptr->energyBend == 0.0);

	/* calculate total energy after looping over every fibre */
	par_ptr->energyTotl = par_ptr->energyStre + par_ptr->energyBend;

	return ierr;
}


void checkVolume(Box *box_ptr, PetscScalar volume)
{
	assert(volume >= 0);
	assert(volume <= box_ptr->xyzDimension[0]*box_ptr->xyzDimension[1]*box_ptr->xyzDimension[2]);
}

PetscScalar calculateVolume(Box *box_ptr)
{
	PetscErrorCode 	ierr = 0;
    PetscScalar 	V = 1.0;
    PetscScalar 	xyzRange[DIMENSION];
    PetscInt 		i;

    for (i = 0; i < DIMENSION; i++)
    {
    	/* cheats here by assuming y always spans height of domain
    	 * WARNING: only true if we fix y=0 and shear in y=height */
	    if (box_ptr->xyzPeriodic[i] == 1 || i == 1)     				/* i periodic */
	    {
	        xyzRange[i] = box_ptr->xyzDimension[i];
	    }
	    else if (box_ptr->xyzPeriodic[i] == 0)          				/* i aperiodic */
	    {
	        /* calculate max and min values for range */
	        xyzRange[i] = calculateAperiodicRange(box_ptr, i);
	    }
	    else
	    {
	        SETERRQ(PETSC_COMM_WORLD,63,"Error in identifying periodicity type");
	    }

	    V *= xyzRange[i]; 	/* starting from 1, multiple by each calculated range */
    }

    checkVolume(box_ptr, V); 	/* basic checks for valid value */

	return V;
}


PetscScalar calculateAperiodicRange(Box *box_ptr, PetscInt dim)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	planarTol = 1e-8;
	PetscScalar 	coordRange;

	/* set some worse case initial coordinate values */
    PetscScalar 	minCoord = box_ptr->xyzDimension[dim];
    PetscScalar 	maxCoord = 0;

    /* add in aperiodic volume calculation */
    PetscInt fID;
    for (fID = 0; fID < box_ptr->fibreCount; fID++)
    {
        Fibre *fibre_ptr = &(box_ptr->masterFibreList[fID]);

        /* find first and last node */
        Node *n0_ptr = fibre_ptr->nodesOnFibreList[0];
        Node *nk_ptr = fibre_ptr->nodesOnFibreList[fibre_ptr->nodesOnFibre-1];

        /* use first node to update min and max coordinate */
        minCoord = minScalar(minCoord, n0_ptr->xyzCoord[dim]);
        maxCoord = maxScalar(maxCoord, n0_ptr->xyzCoord[dim]);

        /* use last node to update min and max coordinate */
        minCoord = minScalar(minCoord, nk_ptr->xyzCoord[dim]);
        maxCoord = maxScalar(maxCoord, nk_ptr->xyzCoord[dim]);
    }
    /* difference max and min to get aperiodic range */
    coordRange = maxCoord - minCoord;

    /* set range to 1 if dealing with a x,y or z plane inside a 3D box */
    if (coordRange < planarTol && coordRange > -planarTol) 
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD,"[WARNING] Detected planar network in 3D.\n");CHKERRQ(ierr);
        coordRange = 1.0;
    }

	return coordRange;
}


PetscErrorCode calculateShearModulus(Box *box_ptr, Parameters *par_ptr)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	V, shearEqn;

	calculateEnergy(box_ptr, par_ptr);CHKERRQ(ierr);

	V = calculateVolume(box_ptr);

    shearEqn = 2 / (V * pow(par_ptr->gamma, 2));

    /* use energy and volume/area to calculate the shear modulus */
	par_ptr->shearModulus = par_ptr->energyTotl * shearEqn;
    par_ptr->shearModAffn = par_ptr->energyAffn * shearEqn;

	return ierr;
}

