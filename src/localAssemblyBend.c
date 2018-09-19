#include "localAssemblyBend.h"

/* Checks for legal kappa calculation arguments */
void checkKappaArguments(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	assert(fIndex >= 0);
}


/* Calculates the bending term kappa */
PetscScalar calculateKappa(Box *box_ptr, Parameters *par_ptr, PetscInt fIndex)
{
	/* validate arguments */
	checkKappaArguments(box_ptr, par_ptr, fIndex);

	PetscScalar radius 	= box_ptr->masterFibreList[fIndex].radius;
    PetscScalar yMod 	= par_ptr->youngsModulus;
    PetscScalar area 	= M_PI * pow(radius, 2);

    return (area * pow(radius, 2) * yMod) / 4.0;
}


/* Adds local bend information for a single fibre to global system */
PetscErrorCode addFibreLocalBend(Box *box_ptr, Parameters *par_ptr, Mat globalMat_H, Vec globalVec_B, PetscInt fIndex)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar		kappa;
	PetscScalar		l_alphOmeg, l_omegBeta, l_alphBeta;
	PetscScalar		bConstNum, bConstDen, bConst;

	Fibre *fibre_ptr = &(box_ptr->masterFibreList[fIndex]);

	/* setup local matrix and rhs vector */
	PetscScalar localBendMat_A[3*DIMENSION][3*DIMENSION];
	PetscScalar localBendVec_b[3*DIMENSION];

	/* setup position vectors */
	PetscScalar s_alph[DIMENSION];
	PetscScalar s_omeg[DIMENSION];
	PetscScalar s_beta[DIMENSION];

	/* setup distance vectors */
	PetscScalar s_alphOmeg[DIMENSION];
	PetscScalar s_omegBeta[DIMENSION];
	PetscScalar s_alphBeta[DIMENSION];

	/* loop over every pair of nodes on the fibre */
	PetscInt i;
	for (i = 0; i < fibre_ptr->nodesOnFibre - 2; i++)
	{
		Node *alph_ptr = fibre_ptr->nodesOnFibreList[i];
		Node *omeg_ptr = fibre_ptr->nodesOnFibreList[i+1];
		Node *beta_ptr = fibre_ptr->nodesOnFibreList[i+2];

        PetscScalar *u_alph = alph_ptr->xyzDisplacement;
        PetscScalar *u_omeg = omeg_ptr->xyzDisplacement;
        PetscScalar *u_beta = beta_ptr->xyzDisplacement;

        /* make position vectors for alpha, omega and beta */
		ierr = makePositionVec(s_alph, alph_ptr);CHKERRQ(ierr);
		ierr = makePositionVec(s_omeg, omeg_ptr);CHKERRQ(ierr);
		ierr = makePositionVec(s_beta, beta_ptr);CHKERRQ(ierr);

		/* make distance vector between position vectors */
		ierr = makeDistanceVec(s_alphOmeg, s_alph, s_omeg, box_ptr);CHKERRQ(ierr);
		ierr = makeDistanceVec(s_omegBeta, s_omeg, s_beta, box_ptr);CHKERRQ(ierr);
		ierr = makeDistanceVec(s_alphBeta, s_alph, s_beta, box_ptr);CHKERRQ(ierr);

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

        if (DIMENSION == 2)
		{
			/* assemble the 2D local matrix and rhs vector */
			ierr = make2DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendMat_A);CHKERRQ(ierr);
			ierr = make2DBendVec(u_alph, u_omeg, u_beta, s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendVec_b);CHKERRQ(ierr);
		}
		else if (DIMENSION == 3)
		{
			/* assemble the 3D local matrix and rhs vector */
			ierr = make3DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendMat_A);CHKERRQ(ierr);
			ierr = make3DBendVec(u_alph, u_omeg, u_beta, s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendVec_b);CHKERRQ(ierr);
		}

		/* determine contributions and add to the global system */
        ierr = addBendContToGlobal( globalMat_H, globalVec_B, box_ptr->nodeInternalCount, 
        							localBendMat_A, localBendVec_b, alph_ptr, omeg_ptr, beta_ptr );
        CHKERRQ(ierr);
	}

	return ierr;
}


/* Assembles the local 2D bend matrix of a given triplet */
PetscErrorCode make2DBendMat(PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta, 
								PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 2D bend RHS vector of a given triplet */
PetscErrorCode make2DBendVec(PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta,
                    			PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta,
                    			PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 3D bend matrix of a given triplet */
PetscErrorCode make3DBendMat(PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta, 
								PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}


/* Assembles the local 3D bend RHS vector of a given triplet */
PetscErrorCode make3DBendVec(PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta,
                    			PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta,
                    			PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

	return ierr;
}
