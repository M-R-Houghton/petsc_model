#include "networkDestroy.h"

/* Initiates network destroy routine */
PetscErrorCode networkDestroy()
{
    PetscErrorCode  ierr;
    ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Destroying network objects...\n");CHKERRQ(ierr);

    return ierr;
}


/* Destroys a parameters structure */
PetscErrorCode destroyParameters(Parameters *par_ptr)
{
    PetscErrorCode  ierr = 0;

    free(par_ptr);
    par_ptr = NULL; assert(par_ptr == NULL);    /* Null check */

    return ierr;
}


/* Destroys a sparse structure */
PetscErrorCode destroySparse(Sparse *sparse_ptr) 
{
    PetscErrorCode  ierr = 0;

    /* free each column of sparse structure */
    free(sparse_ptr->counter);
    sparse_ptr->counter = NULL; assert(sparse_ptr->counter == NULL);
    free(sparse_ptr->rowp);
    sparse_ptr->rowp = NULL; assert(sparse_ptr->rowp == NULL);
    free(sparse_ptr->col);
    sparse_ptr->col = NULL; assert(sparse_ptr->col == NULL);
    free(sparse_ptr->mat);
    sparse_ptr->mat = NULL; assert(sparse_ptr->mat == NULL);

    /* free the remaining sparse structure */
    free(sparse_ptr);
    sparse_ptr = NULL; assert(sparse_ptr == NULL);  /* Null checks */

    return ierr;
}


/* Destroys a box structure */
PetscErrorCode destroyBox(Box *box_ptr)
{
    PetscErrorCode  ierr = 0;

    /* free the master list of nodes */
    free(box_ptr->masterNodeList);
    box_ptr->masterNodeList = NULL; assert(box_ptr->masterNodeList == NULL);

    /* free node list of each fibre */
    int f;
    for (f = 0; f < box_ptr->fibreCount; f++)
    {
        free(box_ptr->masterFibreList[f].nodesOnFibreList); 
        box_ptr->masterFibreList[f].nodesOnFibreList = NULL;
        assert(box_ptr->masterFibreList[f].nodesOnFibreList == NULL);
    }

    /* free the master list of fibres */
    free(box_ptr->masterFibreList);
    box_ptr->masterFibreList = NULL; assert(box_ptr->masterFibreList == NULL);

    /* free the master list of couples */
    free(box_ptr->masterCoupleList);
    box_ptr->masterCoupleList = NULL; assert(box_ptr->masterCoupleList == NULL);
     
    /* free the remaining box */
    free(box_ptr);
    box_ptr = NULL; assert(box_ptr == NULL);    /* Null checks */

    return ierr;
}

