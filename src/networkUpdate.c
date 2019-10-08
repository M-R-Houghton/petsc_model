#include "networkUpdate.h"

/* Updates the network with displacement found from solve */
PetscErrorCode networkUpdate(Box *box_ptr, Vec globalVec_U)
{
	PetscErrorCode ierr = 0;

	/* update all internal nodes before updating dangling nodes */
	PetscInt i;
	for (i = 0; i < box_ptr->nodeCount; i++)
	{
		Node *node_ptr = &(box_ptr->masterNodeList[i]);
		if (node_ptr->nodeType == NODE_INTERNAL)
		{
			ierr = updateInternalNodeDisp(node_ptr, box_ptr->nodeInternalCount, globalVec_U);CHKERRQ(ierr);
		}
	}

	/* NOTE: don't need to update boundary nodes since they are updated
	 * before solving the system */

	/* now update dangling nodes using updated internal nodes */
	PetscInt j;
	for (j = 0; j < box_ptr->fibreCount; j++)
	{
		const Fibre *fibre_ptr = &(box_ptr->masterFibreList[j]);
		const PetscInt nOnFibre = fibre_ptr->nodesOnFibre;

		if (isDanglingFibre(fibre_ptr, nOnFibre))   /* first deal with unlikely case */
        {
            ierr = updateDanglingFibreNodeDisp( fibre_ptr->nodesOnFibreList[0],
                                                fibre_ptr->nodesOnFibreList[1],
                                                fibre_ptr->nodesOnFibreList[2] );
            CHKERRQ(ierr);
        }
        else if (nOnFibre > 2)                      /* now handle the general case */
		{
			/* consider each end of the fibres node list */
			if (fibre_ptr->nodesOnFibreList[0]->nodeType == NODE_DANGLING)
			{
				ierr = updateDanglingNodeDisp(box_ptr, fibre_ptr->nodesOnFibreList[2], 
												fibre_ptr->nodesOnFibreList[1], 
												fibre_ptr->nodesOnFibreList[0]);
				CHKERRQ(ierr);
			}
			if (fibre_ptr->nodesOnFibreList[nOnFibre-1]->nodeType == NODE_DANGLING)
			{
				ierr = updateDanglingNodeDisp(box_ptr, fibre_ptr->nodesOnFibreList[nOnFibre-3], 
												fibre_ptr->nodesOnFibreList[nOnFibre-2], 
												fibre_ptr->nodesOnFibreList[nOnFibre-1]);
				CHKERRQ(ierr);
			}
		}
	}

	return ierr;
}


void checkForValidDisplacement(PetscScalar displacement)
{
    /* exit program early if no sensible solution was found */
    if (isinf(displacement) || isnan(displacement))
    {
        PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Failed to find meaningful solution.\n");
        PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Terminating program early.\n");
        exit(3);    /* this can be changed to generic value of 1 if preferred */
    }
}


/* Updates a single internal node with its relevant solved displacement */
PetscErrorCode updateInternalNodeDisp(Node *node_ptr, PetscInt N, Vec globalVec_U)
{
	PetscErrorCode 	ierr = 0;
	PetscScalar 	*solArray;

	/* NOTE: I need to investigate the performance here.
	 * 	It may be more sensible to call VecGetArray() once before looping over
	 *	all the nodes and call VecRestoreArray() once after leaving the loop. */

	/* Check out array */
    ierr = VecGetArray(globalVec_U, &solArray);CHKERRQ(ierr);
    
    PetscInt i;
    for (i = 0; i < DIMENSION; i++)
    {
    	checkForValidDisplacement(solArray[node_ptr->globalID + i*N]);
    	node_ptr->xyzDisplacement[i] += solArray[node_ptr->globalID + i*N];
    }
    
    /* Check array back in */
    ierr = VecRestoreArray(globalVec_U, &solArray);CHKERRQ(ierr);

	return ierr;
}


/* Checks for dangling fibres that should be avoided during network generation */
PetscBool isDanglingFibre(const Fibre *fibre_ptr, const PetscInt nOnFibre)
{
    PetscErrorCode ierr = 0;
	PetscBool isDangling = PETSC_FALSE;

	if (nOnFibre == 3)
	{
		if (fibre_ptr->nodesOnFibreList[0]->nodeType == NODE_DANGLING &&
			fibre_ptr->nodesOnFibreList[nOnFibre-1]->nodeType == NODE_DANGLING)
		{
			ierr = PetscPrintf(PETSC_COMM_WORLD,"[WARNING] Found dangling fibre!\n");CHKERRQ(ierr);
            isDangling = PETSC_TRUE;
		}
	}

	return isDangling;
}


/* Updates the displacements of dangling nodes of dangling fibres */
PetscErrorCode updateDanglingFibreNodeDisp(Node *bgn, const Node *mid, Node *end)
{
    PetscErrorCode  ierr = 0;
    PetscInt        i;
    for (i = 0; i < DIMENSION; i++)
    {
        bgn->xyzDisplacement[i] = mid->xyzDisplacement[i];
        end->xyzDisplacement[i] = mid->xyzDisplacement[i];
    }
    return ierr;
}


/* Updates a single dangling node using its 2 closest neighbouring nodes */
PetscErrorCode updateDanglingNodeDisp(Box *box_ptr, const Node *alph_ptr, const Node *beta_ptr, Node *delt_ptr)
{
	/* Vector operation summary:						*
	 * 		s_ndel = s_nbet + l_betaDelt * t_nalpBeta 	*
 	 *		u_delt = s_ndel - s_delt 					*/
	PetscErrorCode 	ierr = 0;
	PetscScalar 	l_betaDelt;

    /* should only be updating nodes displacement if dangling */
    assert(delt_ptr->nodeType == NODE_DANGLING);
    assert(alph_ptr->nodeType != NODE_DANGLING);

	/* initialise initial position vectors */
    PetscScalar s_alph[DIMENSION];
    PetscScalar s_beta[DIMENSION];
    PetscScalar s_delt[DIMENSION];

    /* initialise new position vectors */
    PetscScalar s_nalp[DIMENSION];
    PetscScalar s_nbet[DIMENSION];
    PetscScalar s_ndel[DIMENSION];

    /* initialise other vectors */
    PetscScalar s_betaDelt[DIMENSION];
    PetscScalar s_nalpBeta[DIMENSION];
    PetscScalar t_nalpBeta[DIMENSION];

    ierr = makePositionVec(s_alph, alph_ptr);CHKERRQ(ierr);
    ierr = makePositionVec(s_beta, beta_ptr);CHKERRQ(ierr);
    ierr = makePositionVec(s_delt, delt_ptr);CHKERRQ(ierr);

    ierr = updatePositionVec(s_nalp, alph_ptr);CHKERRQ(ierr);
    ierr = updatePositionVec(s_nbet, beta_ptr);CHKERRQ(ierr);

    /* use distance vector to find previous segment length */
    ierr = posVecDifference(s_betaDelt, s_beta, s_delt, box_ptr->xyzPeriodic, box_ptr->xyzDimension);CHKERRQ(ierr);
    l_betaDelt = vecMagnitude(s_betaDelt);

    /* use new distance vector to find new orientation of dangling segment */
    ierr = posVecDifference(s_nalpBeta, s_nalp, s_nbet, box_ptr->xyzPeriodic, box_ptr->xyzDimension);CHKERRQ(ierr);
    ierr = makeTangentVec(t_nalpBeta, s_nalpBeta);CHKERRQ(ierr);

    /* combine new direction with previous magnitude to find new position */
    PetscInt i;
    for (i = 0; i < DIMENSION; i++)
    {
    	/* find new position */
    	s_ndel[i] = s_nbet[i] + (l_betaDelt * t_nalpBeta[i]);
    	/* convert it back to a displacement */
 	 	delt_ptr->xyzDisplacement[i] = s_ndel[i] - s_delt[i];
    }

	return ierr;
}

