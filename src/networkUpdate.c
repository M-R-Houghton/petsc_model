#include "networkUpdate.h"

/* Updates the network with displacement found from solve */
PetscErrorCode networkUpdate(Box *box_ptr, Vec globalVec_U)
{
	PetscErrorCode ierr = 0;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Updating network...\n");CHKERRQ(ierr);

	ierr = VecView(globalVec_U,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

	/* update network by looping node by node */
	PetscInt i;
	for (i = 0; i < box_ptr->nodeCount; i++)
	{
		Node *node_ptr = &(box_ptr->masterNodeList[i]);

		/* update all internal nodes before updating dangling nodes */
		if (node_ptr->nodeType == NODE_INTERNAL)
		{
			updateInternalNodeDisp(node_ptr, box_ptr->nodeInternalCount, globalVec_U);
		}

		/* NOTE: don't need to update boundary nodes since they are updated
		 * before solving the system */
	}

	/* now update dangling nodes using updated internal nodes */

	return ierr;
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
    	node_ptr->xyzDisplacement[i] += solArray[node_ptr->globalID + i*N];
    }
    
    /* Check array back in */
    ierr = VecRestoreArray(globalVec_U, &solArray);CHKERRQ(ierr);

	return ierr;
}