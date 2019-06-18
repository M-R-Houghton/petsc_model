#include "localAssemblyBend.h"

/* Calculates the bending term kappa */
PetscScalar calculateKappa(const PetscScalar radius, const PetscScalar youngsModulus)
{
    PetscScalar area = M_PI * pow(radius, 2);
    return (area * pow(radius, 2) * youngsModulus) / 4.0;
}


/* Adds local bend information for a single fibre to global system */
PetscErrorCode addFibreLocalBend(Mat globalMat_H, Vec globalVec_B, const PetscInt N, const Fibre *fibre_ptr, 
                                    const PetscInt *xyzPer, const PetscScalar *xyzDim, const PetscScalar youngsModulus)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar		kappa;
	PetscScalar		l_alphOmeg, l_omegBeta, l_alphBeta;
	PetscScalar		bConstNum, bConstDen, bConst;

	/* setup local matrix and rhs vector */
	PetscScalar localBendMat_A[9][9];
	PetscScalar localBendVec_b[9];         /* actual size will be 3*DIMENSION */

	/* setup difference vectors */
	PetscScalar s_alphOmeg[DIMENSION], u_alphOmeg[DIMENSION];
	PetscScalar s_omegBeta[DIMENSION], u_omegBeta[DIMENSION];
	PetscScalar s_alphBeta[DIMENSION], u_alphBeta[DIMENSION];

	/* loop over every pair of nodes on the fibre */
	PetscInt i;
	for (i = 0; i < fibre_ptr->nodesOnFibre - 2; i++)
	{
		const Node *n_alph = fibre_ptr->nodesOnFibreList[i];
		const Node *n_omeg = fibre_ptr->nodesOnFibreList[i+1];
		const Node *n_beta = fibre_ptr->nodesOnFibreList[i+2];

		/* make difference vectors from position vectors with boundary checking */
		ierr = posVecDifference(s_alphOmeg, n_alph->xyzCoord, n_omeg->xyzCoord, xyzPer, xyzDim);CHKERRQ(ierr);
		ierr = posVecDifference(s_omegBeta, n_omeg->xyzCoord, n_beta->xyzCoord, xyzPer, xyzDim);CHKERRQ(ierr);
		ierr = posVecDifference(s_alphBeta, n_alph->xyzCoord, n_beta->xyzCoord, xyzPer, xyzDim);CHKERRQ(ierr);

		/* make difference vectors from displacement vectors */
		ierr = stdVecDifference(u_alphOmeg, n_alph->xyzDisplacement, n_omeg->xyzDisplacement);CHKERRQ(ierr);
		ierr = stdVecDifference(u_omegBeta, n_omeg->xyzDisplacement, n_beta->xyzDisplacement);CHKERRQ(ierr);
		ierr = stdVecDifference(u_alphBeta, n_alph->xyzDisplacement, n_beta->xyzDisplacement);CHKERRQ(ierr);

		/* calculate segment lengths */
		l_alphOmeg = vecMagnitude(s_alphOmeg);
		l_omegBeta = vecMagnitude(s_omegBeta);	/* WARNING: do NOT assume that 			 */
		l_alphBeta = l_alphOmeg + l_omegBeta;	/* l_alphBeta = vecMagnitude(s_alphBeta) */

		/* calculate bending modulus kappa */
        kappa = calculateKappa(fibre_ptr->radius, youngsModulus);

		/* calculate bending constant */
		bConstNum = 2 * kappa;
		bConstDen = l_alphBeta * pow(l_alphOmeg,2) * pow(l_omegBeta,2);
		bConst 	  = bConstNum / bConstDen;

        if (DIMENSION == 2)
		{
			/* assemble the 2D local matrix and rhs vector */
			ierr = make2DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendMat_A);CHKERRQ(ierr);
			ierr = make2DBendVec(u_alphOmeg, u_omegBeta, s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendVec_b);
            CHKERRQ(ierr);
		}
		else if (DIMENSION == 3)
		{
			/* assemble the 3D local matrix and rhs vector */
			ierr = make3DBendMat(s_alphOmeg, s_omegBeta, s_alphBeta, bConst, localBendMat_A);CHKERRQ(ierr);
			ierr = make3DBendVec(u_alphOmeg, u_omegBeta, u_alphBeta, s_alphOmeg, 
                                    s_omegBeta, s_alphBeta, bConst, localBendVec_b);
            CHKERRQ(ierr);
		}

		/* determine contributions and add to the global system */
        ierr = addBendContToGlobal( globalMat_H, globalVec_B, N,  
                                    n_alph->globalID, n_omeg->globalID, n_beta->globalID,
                                    n_alph->nodeType, n_omeg->nodeType, n_beta->nodeType,
        							localBendMat_A, localBendVec_b );
        CHKERRQ(ierr);
	}

	return ierr;
}


/* Assembles the local 2D bend matrix of a given triplet */
PetscErrorCode make2DBendMat(const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                const PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
    PetscErrorCode ierr = 0;

    const PetscInt x = 0;    /* set these values purely for readability */
    const PetscInt y = 1;    /* set *(1) also for readability */

    /* Row (alpha) X */
    localBendMat_A[0][0] = ( 1) * bConst * (pow(s_omegBeta[y], 2));
    localBendMat_A[0][1] = (-1) * bConst * (s_omegBeta[y]*s_alphBeta[y]);
    localBendMat_A[0][2] = ( 1) * bConst * (s_omegBeta[y]*s_alphOmeg[y]);
    localBendMat_A[0][3] = (-1) * bConst * (s_omegBeta[y]*s_omegBeta[x]);
    localBendMat_A[0][4] = ( 1) * bConst * (s_omegBeta[y]*s_alphBeta[x]);
    localBendMat_A[0][5] = (-1) * bConst * (s_omegBeta[y]*s_alphOmeg[x]);

    /* Row (omega) X */
    localBendMat_A[1][0] = localBendMat_A[0][1];
    localBendMat_A[1][1] = ( 1) * bConst * (pow(s_alphBeta[y], 2));
    localBendMat_A[1][2] = (-1) * bConst * (s_alphBeta[y]*s_alphOmeg[y]);
    localBendMat_A[1][3] = ( 1) * bConst * (s_alphBeta[y]*s_omegBeta[x]);
    localBendMat_A[1][4] = (-1) * bConst * (s_alphBeta[y]*s_alphBeta[x]);
    localBendMat_A[1][5] = ( 1) * bConst * (s_alphBeta[y]*s_alphOmeg[x]);

    /* Row (beta) X */
    localBendMat_A[2][0] = localBendMat_A[0][2];
    localBendMat_A[2][1] = localBendMat_A[1][2];
    localBendMat_A[2][2] = ( 1) * bConst * (pow(s_alphOmeg[y], 2));
    localBendMat_A[2][3] = (-1) * bConst * (s_alphOmeg[y]*s_omegBeta[x]);
    localBendMat_A[2][4] = ( 1) * bConst * (s_alphOmeg[y]*s_alphBeta[x]);
    localBendMat_A[2][5] = (-1) * bConst * (s_alphOmeg[y]*s_alphOmeg[x]);

    /* Row (alpha) Y */
    localBendMat_A[3][0] = localBendMat_A[0][3];
    localBendMat_A[3][1] = localBendMat_A[1][3];
    localBendMat_A[3][2] = localBendMat_A[2][3];
    localBendMat_A[3][3] = ( 1) * bConst * (pow(s_omegBeta[x], 2));
    localBendMat_A[3][4] = (-1) * bConst * (s_omegBeta[x]*s_alphBeta[x]);
    localBendMat_A[3][5] = ( 1) * bConst * (s_omegBeta[x]*s_alphOmeg[x]);

    /* Row (omega) Y */
    localBendMat_A[4][0] = localBendMat_A[0][4];
    localBendMat_A[4][1] = localBendMat_A[1][4];
    localBendMat_A[4][2] = localBendMat_A[2][4];
    localBendMat_A[4][3] = localBendMat_A[3][4];
    localBendMat_A[4][4] = ( 1) * bConst * (pow(s_alphBeta[x], 2));
    localBendMat_A[4][5] = (-1) * bConst * (s_alphBeta[x]*s_alphOmeg[x]);

    /* Row (beta) Y */
    localBendMat_A[5][0] = localBendMat_A[0][5];
    localBendMat_A[5][1] = localBendMat_A[1][5];
    localBendMat_A[5][2] = localBendMat_A[2][5];
    localBendMat_A[5][3] = localBendMat_A[3][5];
    localBendMat_A[5][4] = localBendMat_A[4][5];
    localBendMat_A[5][5] = ( 1) * bConst * (pow(s_alphOmeg[x], 2));

    /* 
     * NOTE: The remaining values are not set and should be ignored
     *  localBendMat_A[0:5][6:8] = N/A
     *  localBendMat_A[6:8][0:8] = N/A
     */

	return ierr;
}


/* Assembles the local 2D bend RHS vector of a given triplet */
PetscErrorCode make2DBendVec( const PetscScalar *u_alphOmeg, const PetscScalar *u_omegBeta,
                    			const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

    const PetscInt x = 0;     /* set these values purely for readability */
    const PetscInt y = 1;     /* set *(1) also for readability */

    PetscScalar phi_xy = s_alphOmeg[x] * u_omegBeta[y] - s_alphOmeg[y] * u_omegBeta[x]
                        - s_omegBeta[x] * u_alphOmeg[y] + s_omegBeta[y] * u_alphOmeg[x];

    /* we want the negation of the 1st partial derivatives */
    phi_xy *= -1;       /* This should be done in a cleaner way!!! */

    localBendVec_b[0] = bConst * ((-1) * phi_xy * s_omegBeta[y]);
    localBendVec_b[1] = bConst * (( 1) * phi_xy * s_alphBeta[y]);
    localBendVec_b[2] = bConst * ((-1) * phi_xy * s_alphOmeg[y]);
    localBendVec_b[3] = bConst * (( 1) * phi_xy * s_omegBeta[x]);
    localBendVec_b[4] = bConst * ((-1) * phi_xy * s_alphBeta[x]);
    localBendVec_b[5] = bConst * (( 1) * phi_xy * s_alphOmeg[x]);

    /* 
     * NOTE: The remaining values are not set and should be ignored
     *  localBendVec_b[6:8] = N/A
     */

	return ierr;
}


/* Assembles the local 3D bend matrix of a given triplet */
PetscErrorCode make3DBendMat(const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                const PetscScalar bConst, PetscScalar localBendMat_A[9][9] )
{
	PetscErrorCode ierr = 0;

	const PetscInt x = 0;    /* set these values purely for readability */
    const PetscInt y = 1;    /* set *(1) also for readability */
    const PetscInt z = 2;

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
PetscErrorCode make3DBendVec(const PetscScalar *u_alphOmeg, const PetscScalar *u_omegBeta, const PetscScalar *u_alphBeta,
                    			const PetscScalar *s_alphOmeg, const PetscScalar *s_omegBeta, const PetscScalar *s_alphBeta, 
                                PetscScalar bConst, PetscScalar localBendVec_b[9] )
{
	PetscErrorCode ierr = 0;

	const PetscInt x = 0;    /* set these values purely for readability */
    const PetscInt y = 1;    /* set *(1) also for readability */
    const PetscInt z = 2;

    PetscScalar phi_xy = s_alphOmeg[x] * u_omegBeta[y] - s_alphOmeg[y] * u_omegBeta[x]
                    	- s_omegBeta[x] * u_alphOmeg[y] + s_omegBeta[y] * u_alphOmeg[x];

    PetscScalar phi_zx = s_alphOmeg[z] * u_omegBeta[x] - s_alphOmeg[x] * u_omegBeta[z]
                    	- s_omegBeta[z] * u_alphOmeg[x] + s_omegBeta[x] * u_alphOmeg[z];

    PetscScalar phi_yz = s_alphOmeg[y] * u_omegBeta[z] - s_alphOmeg[z] * u_omegBeta[y]
                    	- s_omegBeta[y] * u_alphOmeg[z] + s_omegBeta[z] * u_alphOmeg[y];

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


