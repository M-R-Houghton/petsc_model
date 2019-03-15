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
	PetscScalar localBendMat_A[9][9];
	PetscScalar localBendVec_b[9];         /* actual size will be 3*DIMENSION */

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

        //printf("bConst: %0.16g\n", bConst);
        //printf("l_alphOmeg: %0.16g\n", l_alphOmeg);
        //printf("l_omegBeta: %0.16g\n", l_omegBeta);
        //printf("l_alphBeta: %0.16g\n", l_alphBeta);

        /*
        int i;
        for (i = 0; i < DIMENSION; i++)
        {
            printf("s_alphOmeg: %0.16g\n", s_alphOmeg[i]);
        }
        for (i = 0; i < DIMENSION; i++)
        {
            printf("s_omegBeta: %0.16g\n", s_omegBeta[i]);
        }
        for (i = 0; i < DIMENSION; i++)
        {
            printf("s_alphBeta: %0.16g\n", s_alphBeta[i]);
        }
        */

        /* for debugging purposes */
        /*
        for (i = 0; i < 9; i++)
        {
            int j;
            for (j = 0; j < 9; j++)
            {
                ierr = PetscPrintf(PETSC_COMM_WORLD,"%0.8g\t", localBendMat_A[i][j]);CHKERRQ(ierr);
            }
            ierr = PetscPrintf(PETSC_COMM_WORLD,"\n");CHKERRQ(ierr);
        }
        */

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

    PetscInt x = 0;    /* set these values purely for readability */
    PetscInt y = 1;    /* set *(1) also for readability */

    /* Row (alpha) X */
    localBendMat_A[0][0] = ( 1) * bConst * (pow(s_omegBeta[y], 2));
    localBendMat_A[0][1] = (-1) * bConst * (s_omegBeta[y]*s_alphBeta[y]);
    localBendMat_A[0][2] = ( 1) * bConst * (s_omegBeta[y]*s_alphOmeg[y]);
    localBendMat_A[0][3] = (-1) * bConst * (s_omegBeta[y]*s_omegBeta[x]);
    localBendMat_A[0][4] = ( 1) * bConst * (s_omegBeta[y]*s_alphBeta[x]);
    localBendMat_A[0][5] = (-1) * bConst * (s_omegBeta[y]*s_alphOmeg[x]);
/*  localBendMat_A[0][6] = 0; 
 *  localBendMat_A[0][7] = 0;
 *  localBendMat_A[0][8] = 0; */

    /* Row (omega) X */
    localBendMat_A[1][0] = localBendMat_A[0][1];
    localBendMat_A[1][1] = ( 1) * bConst * (pow(s_alphBeta[y], 2));
    localBendMat_A[1][2] = (-1) * bConst * (s_alphBeta[y]*s_alphOmeg[y]);
    localBendMat_A[1][3] = ( 1) * bConst * (s_alphBeta[y]*s_omegBeta[x]);
    localBendMat_A[1][4] = (-1) * bConst * (s_alphBeta[y]*s_alphBeta[x]);
    localBendMat_A[1][5] = ( 1) * bConst * (s_alphBeta[y]*s_alphOmeg[x]);
/*  localBendMat_A[1][6] = 0; 
 *  localBendMat_A[1][7] = 0;
 *  localBendMat_A[1][8] = 0; */

    /* Row (beta) X */
    localBendMat_A[2][0] = localBendMat_A[0][2];
    localBendMat_A[2][1] = localBendMat_A[1][2];
    localBendMat_A[2][2] = ( 1) * bConst * (pow(s_alphOmeg[y], 2));
    localBendMat_A[2][3] = (-1) * bConst * (s_alphOmeg[y]*s_omegBeta[x]);
    localBendMat_A[2][4] = ( 1) * bConst * (s_alphOmeg[y]*s_alphBeta[x]);
    localBendMat_A[2][5] = (-1) * bConst * (s_alphOmeg[y]*s_alphOmeg[x]);
/*  localBendMat_A[2][6] = 0; 
 *  localBendMat_A[2][7] = 0;
 *  localBendMat_A[2][8] = 0; */

    /* Row (alpha) Y */
    localBendMat_A[3][0] = localBendMat_A[0][3];
    localBendMat_A[3][1] = localBendMat_A[1][3];
    localBendMat_A[3][2] = localBendMat_A[2][3];
    localBendMat_A[3][3] = ( 1) * bConst * (pow(s_omegBeta[x], 2));
    localBendMat_A[3][4] = (-1) * bConst * (s_omegBeta[x]*s_alphBeta[x]);
    localBendMat_A[3][5] = ( 1) * bConst * (s_omegBeta[x]*s_alphOmeg[x]);
/*  localBendMat_A[3][6] = 0; 
 *  localBendMat_A[3][7] = 0;
 *  localBendMat_A[3][8] = 0; */

    /* Row (omega) Y */
    localBendMat_A[4][0] = localBendMat_A[0][4];
    localBendMat_A[4][1] = localBendMat_A[1][4];
    localBendMat_A[4][2] = localBendMat_A[2][4];
    localBendMat_A[4][3] = localBendMat_A[3][4];
    localBendMat_A[4][4] = ( 1) * bConst * (pow(s_alphBeta[x], 2));
    localBendMat_A[4][5] = (-1) * bConst * (s_alphBeta[x]*s_alphOmeg[x]);
/*  localBendMat_A[4][6] = 0; 
 *  localBendMat_A[4][7] = 0;
 *  localBendMat_A[4][8] = 0; */

    /* Row (beta) Y */
    localBendMat_A[5][0] = localBendMat_A[0][5];
    localBendMat_A[5][1] = localBendMat_A[1][5];
    localBendMat_A[5][2] = localBendMat_A[2][5];
    localBendMat_A[5][3] = localBendMat_A[3][5];
    localBendMat_A[5][4] = localBendMat_A[4][5];
    localBendMat_A[5][5] = ( 1) * bConst * (pow(s_alphOmeg[x], 2));
/*  localBendMat_A[5][6] = 0; 
 *  localBendMat_A[5][7] = 0;
 *  localBendMat_A[5][8] = 0; */

    /* Row (alpha) Z */
/*  localBendMat_A[6][0] = 0;
 *  localBendMat_A[6][1] = 0;
 *  localBendMat_A[6][2] = 0;
 *  localBendMat_A[6][3] = 0;
 *  localBendMat_A[6][4] = 0;
 *  localBendMat_A[6][5] = 0;
 *  localBendMat_A[6][6] = 0;
 *  localBendMat_A[6][7] = 0;
 *  localBendMat_A[6][8] = 0; */

    /* Row (omega) Z */
/*  localBendMat_A[7][0] = 0;
 *  localBendMat_A[7][1] = 0;
 *  localBendMat_A[7][2] = 0;
 *  localBendMat_A[7][3] = 0;
 *  localBendMat_A[7][4] = 0;
 *  localBendMat_A[7][5] = 0;
 *  localBendMat_A[7][6] = 0;
 *  localBendMat_A[7][7] = 0;
 *  localBendMat_A[7][8] = 0; */

    /* Row (beta) Z */
/*  localBendMat_A[8][0] = 0;
 *  localBendMat_A[8][1] = 0;
 *  localBendMat_A[8][2] = 0;
 *  localBendMat_A[8][3] = 0;
 *  localBendMat_A[8][4] = 0;
 *  localBendMat_A[8][5] = 0;
 *  localBendMat_A[8][6] = 0;
 *  localBendMat_A[8][7] = 0;
 *  localBendMat_A[8][8] = 0; */

	return ierr;
}


/* Assembles the local 2D bend RHS vector of a given triplet */
PetscErrorCode make2DBendVec(PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta,
                    			PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta,
                    			PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

    PetscInt x = 0;     /* set these values purely for readability */
    PetscInt y = 1;     /* set *(1) also for readability */

    PetscScalar phi_xy = s_alphOmeg[x] * (u_beta[y] - u_omeg[y])
                        - s_alphOmeg[y] * (u_beta[x] - u_omeg[x])
                        - s_omegBeta[x] * (u_omeg[y] - u_alph[y])
                        + s_omegBeta[y] * (u_omeg[x] - u_alph[x]);
/*  PetscScalar phi_zx = 0;
 *  PetscScalar phi_yz = 0; */

    /* we want the negation of the 1st partial derivatives */
    phi_xy *= -1;       /* This should be done in a cleaner way!!! */

    localBendVec_b[0] = bConst * ((-1) * phi_xy * s_omegBeta[y]);
    localBendVec_b[1] = bConst * (( 1) * phi_xy * s_alphBeta[y]);
    localBendVec_b[2] = bConst * ((-1) * phi_xy * s_alphOmeg[y]);
    localBendVec_b[3] = bConst * (( 1) * phi_xy * s_omegBeta[x]);
    localBendVec_b[4] = bConst * ((-1) * phi_xy * s_alphBeta[x]);
    localBendVec_b[5] = bConst * (( 1) * phi_xy * s_alphOmeg[x]);
/*  localBendVec_b[6] = 0;
 *  localBendVec_b[7] = 0;
 *  localBendVec_b[8] = 0; */

	return ierr;
}


/* Assembles the local 3D bend matrix of a given triplet */
PetscErrorCode make3DBendMat(PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta, 
								PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
	PetscErrorCode ierr = 0;

	PetscInt x = 0;    /* set these values purely for readability */
    PetscInt y = 1;    /* set *(1) also for readability */
    PetscInt z = 2;

    /* Row (alpha) X */
    localBendMat_A[0][0] = ( 1) * bConst * (pow(s_omegBeta[y], 2) + pow(s_omegBeta[z],2));
    localBendMat_A[0][1] = (-1) * bConst * (s_omegBeta[y]*s_alphBeta[y] + s_omegBeta[z]*s_alphBeta[z]);
    localBendMat_A[0][2] = ( 1) * bConst * (s_omegBeta[y]*s_alphOmeg[y] + s_omegBeta[z]*s_alphOmeg[z]);

    localBendMat_A[0][3] = (-1) * bConst * (s_omegBeta[y]*s_omegBeta[x]);
    localBendMat_A[0][4] = ( 1) * bConst * (s_omegBeta[y]*s_alphBeta[x]);
    localBendMat_A[0][5] = (-1) * bConst * (s_omegBeta[y]*s_alphOmeg[x]);

    localBendMat_A[0][6] = (-1) * bConst * (s_omegBeta[z]*s_omegBeta[x]);
    localBendMat_A[0][7] = ( 1) * bConst * (s_omegBeta[z]*s_alphBeta[x]);
    localBendMat_A[0][8] = (-1) * bConst * (s_omegBeta[z]*s_alphOmeg[x]);


    /* Row (omega) X */
    localBendMat_A[1][0] = localBendMat_A[0][1];
    localBendMat_A[1][1] = ( 1) * bConst * (pow(s_alphBeta[y], 2) + pow(s_alphBeta[z], 2));
    localBendMat_A[1][2] = (-1) * bConst * (s_alphBeta[y]*s_alphOmeg[y] + s_alphBeta[z]*s_alphOmeg[z]);

    localBendMat_A[1][3] = ( 1) * bConst * (s_alphBeta[y]*s_omegBeta[x]);
    localBendMat_A[1][4] = (-1) * bConst * (s_alphBeta[y]*s_alphBeta[x]);
    localBendMat_A[1][5] = ( 1) * bConst * (s_alphBeta[y]*s_alphOmeg[x]);

    localBendMat_A[1][6] = ( 1) * bConst * (s_alphBeta[z]*s_omegBeta[x]);
    localBendMat_A[1][7] = (-1) * bConst * (s_alphBeta[z]*s_alphBeta[x]);
    localBendMat_A[1][8] = ( 1) * bConst * (s_alphBeta[z]*s_alphOmeg[x]);


    /* Row (beta) X */
    localBendMat_A[2][0] = localBendMat_A[0][2];
    localBendMat_A[2][1] = localBendMat_A[1][2];
    localBendMat_A[2][2] = ( 1) * bConst * (pow(s_alphOmeg[y], 2) + pow(s_alphOmeg[z], 2));

    localBendMat_A[2][3] = (-1) * bConst * (s_alphOmeg[y]*s_omegBeta[x]);
    localBendMat_A[2][4] = ( 1) * bConst * (s_alphOmeg[y]*s_alphBeta[x]);
    localBendMat_A[2][5] = (-1) * bConst * (s_alphOmeg[y]*s_alphOmeg[x]);

    localBendMat_A[2][6] = (-1) * bConst * (s_alphOmeg[z]*s_omegBeta[x]);
    localBendMat_A[2][7] = ( 1) * bConst * (s_alphOmeg[z]*s_alphBeta[x]);
    localBendMat_A[2][8] = (-1) * bConst * (s_alphOmeg[z]*s_alphOmeg[x]);


    /* Row (alpha) Y */
    localBendMat_A[3][0] = localBendMat_A[0][3];
    localBendMat_A[3][1] = localBendMat_A[1][3];
    localBendMat_A[3][2] = localBendMat_A[2][3];

    localBendMat_A[3][3] = ( 1) * bConst * (pow(s_omegBeta[z], 2) + pow(s_omegBeta[x], 2));
    localBendMat_A[3][4] = (-1) * bConst * (s_omegBeta[z]*s_alphBeta[z] + s_omegBeta[x]*s_alphBeta[x]);
    localBendMat_A[3][5] = ( 1) * bConst * (s_omegBeta[z]*s_alphOmeg[z] + s_omegBeta[x]*s_alphOmeg[x]);

    localBendMat_A[3][6] = (-1) * bConst * (s_omegBeta[z]*s_omegBeta[y]);
    localBendMat_A[3][7] = ( 1) * bConst * (s_omegBeta[z]*s_alphBeta[y]);
    localBendMat_A[3][8] = (-1) * bConst * (s_omegBeta[z]*s_alphOmeg[y]);


    /* Row (omega) Y */
    localBendMat_A[4][0] = localBendMat_A[0][4];
    localBendMat_A[4][1] = localBendMat_A[1][4];
    localBendMat_A[4][2] = localBendMat_A[2][4];

    localBendMat_A[4][3] = localBendMat_A[3][4];
    localBendMat_A[4][4] = ( 1) * bConst * (pow(s_alphBeta[z], 2) + pow(s_alphBeta[x], 2));
    localBendMat_A[4][5] = (-1) * bConst * (s_alphBeta[z]*s_alphOmeg[z] + s_alphBeta[x]*s_alphOmeg[x]);

    localBendMat_A[4][6] = ( 1) * bConst * (s_alphBeta[z]*s_omegBeta[y]);
    localBendMat_A[4][7] = (-1) * bConst * (s_alphBeta[z]*s_alphBeta[y]);
    localBendMat_A[4][8] = ( 1) * bConst * (s_alphBeta[z]*s_alphOmeg[y]);


    /* Row (beta) Y */
    localBendMat_A[5][0] = localBendMat_A[0][5];
    localBendMat_A[5][1] = localBendMat_A[1][5];
    localBendMat_A[5][2] = localBendMat_A[2][5];

    localBendMat_A[5][3] = localBendMat_A[3][5];
    localBendMat_A[5][4] = localBendMat_A[4][5];
    localBendMat_A[5][5] = ( 1) * bConst * (pow(s_alphOmeg[z], 2) + pow(s_alphOmeg[x], 2));

    localBendMat_A[5][6] = (-1) * bConst * (s_alphOmeg[z]*s_omegBeta[y]);
    localBendMat_A[5][7] = ( 1) * bConst * (s_alphOmeg[z]*s_alphBeta[y]);
    localBendMat_A[5][8] = (-1) * bConst * (s_alphOmeg[z]*s_alphOmeg[y]);


    /* Row (alpha) Z */
    localBendMat_A[6][0] = localBendMat_A[0][6];
    localBendMat_A[6][1] = localBendMat_A[1][6];
    localBendMat_A[6][2] = localBendMat_A[2][6];

    localBendMat_A[6][3] = localBendMat_A[3][6];
    localBendMat_A[6][4] = localBendMat_A[4][6];
    localBendMat_A[6][5] = localBendMat_A[5][6];

    localBendMat_A[6][6] = ( 1) * bConst * (pow(s_omegBeta[y], 2) + pow(s_omegBeta[x], 2));
    localBendMat_A[6][7] = (-1) * bConst * (s_omegBeta[y]*s_alphBeta[y] + s_omegBeta[x]*s_alphBeta[x]);
    localBendMat_A[6][8] = ( 1) * bConst * (s_omegBeta[y]*s_alphOmeg[y] + s_omegBeta[x]*s_alphOmeg[x]);


    /* Row (omega) Z */
    localBendMat_A[7][0] = localBendMat_A[0][7];
    localBendMat_A[7][1] = localBendMat_A[1][7];
    localBendMat_A[7][2] = localBendMat_A[2][7];

    localBendMat_A[7][3] = localBendMat_A[3][7];
    localBendMat_A[7][4] = localBendMat_A[4][7];
    localBendMat_A[7][5] = localBendMat_A[5][7];

    localBendMat_A[7][6] = localBendMat_A[6][7];
    localBendMat_A[7][7] = ( 1) * bConst * (pow(s_alphBeta[y], 2) + pow(s_alphBeta[x], 2));
    localBendMat_A[7][8] = (-1) * bConst * (s_alphBeta[y]*s_alphOmeg[y] + s_alphBeta[x]*s_alphOmeg[x]);


    /* Row (beta) Z */
    localBendMat_A[8][0] = localBendMat_A[0][8];
    localBendMat_A[8][1] = localBendMat_A[1][8];
    localBendMat_A[8][2] = localBendMat_A[2][8];

    localBendMat_A[8][3] = localBendMat_A[3][8];
    localBendMat_A[8][4] = localBendMat_A[4][8];
    localBendMat_A[8][5] = localBendMat_A[5][8];

    localBendMat_A[8][6] = localBendMat_A[6][8];
    localBendMat_A[8][7] = localBendMat_A[7][8];
    localBendMat_A[8][8] = ( 1) * bConst * (pow(s_alphOmeg[y], 2) + pow(s_alphOmeg[y], 2));

	return ierr;
}


/* Assembles the local 3D bend RHS vector of a given triplet */
PetscErrorCode make3DBendVec(PetscScalar *u_alph, PetscScalar *u_omeg, PetscScalar *u_beta,
                    			PetscScalar *s_alphOmeg, PetscScalar *s_omegBeta,
                    			PetscScalar *s_alphBeta, PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

	PetscInt x = 0;    /* set these values purely for readability */
    PetscInt y = 1;    /* set *(1) also for readability */
    PetscInt z = 2;

    PetscScalar phi_xy = s_alphOmeg[x] * (u_beta[y] - u_omeg[y])
                    	- s_alphOmeg[y] * (u_beta[x] - u_omeg[x])
                    	- s_omegBeta[x] * (u_omeg[y] - u_alph[y])
                    	+ s_omegBeta[y] * (u_omeg[x] - u_alph[x]);

    PetscScalar phi_zx = s_alphOmeg[z] * (u_beta[x] - u_omeg[x])
                    	- s_alphOmeg[x] * (u_beta[z] - u_omeg[z])
                    	- s_omegBeta[z] * (u_omeg[x] - u_alph[x])
                    	+ s_omegBeta[x] * (u_omeg[z] - u_alph[z]);

    PetscScalar phi_yz = s_alphOmeg[y] * (u_beta[z] - u_omeg[z])
                    	- s_alphOmeg[z] * (u_beta[y] - u_omeg[y])
                    	- s_omegBeta[y] * (u_omeg[z] - u_alph[z])
                    	+ s_omegBeta[z] * (u_omeg[y] - u_alph[y]);

    /* we want the negation of the 1st partial derivatives */
    phi_xy *= -1;
    phi_zx *= -1;
    phi_yz *= -1;     /* This should be done in a cleaner way!!! */

    localBendVec_b[0] = bConst * ((-1) * phi_xy * s_omegBeta[y] + ( 1) * phi_zx * s_omegBeta[z]);
    localBendVec_b[1] = bConst * (( 1) * phi_xy * s_alphBeta[y] + (-1) * phi_zx * s_alphBeta[z]);
    localBendVec_b[2] = bConst * ((-1) * phi_xy * s_alphOmeg[y] + ( 1) * phi_zx * s_alphOmeg[z]);
    localBendVec_b[3] = bConst * ((-1) * phi_yz * s_omegBeta[z] + ( 1) * phi_xy * s_omegBeta[x]);
    localBendVec_b[4] = bConst * (( 1) * phi_yz * s_alphBeta[z] + (-1) * phi_xy * s_alphBeta[x]);
    localBendVec_b[5] = bConst * ((-1) * phi_yz * s_alphOmeg[z] + ( 1) * phi_xy * s_alphOmeg[x]);
    localBendVec_b[6] = bConst * ((-1) * phi_zx * s_omegBeta[x] + ( 1) * phi_yz * s_omegBeta[y]);
    localBendVec_b[7] = bConst * (( 1) * phi_zx * s_alphBeta[x] + (-1) * phi_yz * s_alphBeta[y]);
    localBendVec_b[8] = bConst * ((-1) * phi_zx * s_alphOmeg[x] + ( 1) * phi_yz * s_alphOmeg[y]);

	return ierr;
}
