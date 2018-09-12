#include "networkWrite.h"

/* Initiates network write out routine */
PetscErrorCode networkWrite(const char *fileName, Box *box_ptr)
{
	PetscErrorCode 	ierr;
	PetscInt fIndex, nIndex;
	FILE *file_ptr;

	ierr = PetscPrintf(PETSC_COMM_WORLD,"[STATUS] Writing to file...\n");CHKERRQ(ierr);

	file_ptr = fopen(fileName, "w+");

	/* first written line should be box line */
	writeBoxLine(file_ptr, box_ptr);

	/* followed a line for every fibre */
	for (fIndex = 0; fIndex < box_ptr->fibreCount; fIndex++)
	{
		writeFibreLine(file_ptr, &(box_ptr->masterFibreList[fIndex]), fIndex);
	}

	/* ending with a line for every node */
	for (nIndex = 0; nIndex < box_ptr->nodeCount; nIndex++)
	{
		writeNodeLine(file_ptr, &(box_ptr->masterNodeList[nIndex]), nIndex);
	}

	fclose(file_ptr);

	return ierr;
}


/* Writes box information to file */
PetscErrorCode writeBoxLine(FILE *file_ptr, Box *box_ptr)
{
	PetscErrorCode ierr = 0;

	fprintf(file_ptr, "b ");
	fprintf(file_ptr, "%d ", box_ptr->nodeCount);
	fprintf(file_ptr, "%d ", box_ptr->fibreCount);
	fprintf(file_ptr, "%lf ", box_ptr->xyzDimension[0]);
	fprintf(file_ptr, "%lf ", box_ptr->xyzDimension[1]);
	fprintf(file_ptr, "%lf ", box_ptr->xyzDimension[2]);
	fprintf(file_ptr, "%d ", box_ptr->xyzPeriodic[0]);
	fprintf(file_ptr, "%d ", box_ptr->xyzPeriodic[1]);
	fprintf(file_ptr, "%d\n", box_ptr->xyzPeriodic[2]);

	return ierr;
}


/* Writes fibre information to file */
PetscErrorCode writeFibreLine(FILE *file_ptr, Fibre *fibre_ptr, PetscInt fIndex)
{
	PetscErrorCode ierr = 0;

	fprintf(file_ptr, "f ");
	fprintf(file_ptr, "%d ", fibre_ptr->fibreID);
	fprintf(file_ptr, "%lf ", fibre_ptr->radius);

	int nOnFibre;
	for (nOnFibre = 0; nOnFibre < fibre_ptr->nodesOnFibre; nOnFibre++)
	{
		fprintf(file_ptr, "%d ", fibre_ptr->nodesOnFibreList[nOnFibre]->nodeID);
	}
	fprintf(file_ptr, "\n");

	return ierr;
}


/* Writes node information to file */
PetscErrorCode writeNodeLine(FILE *file_ptr, Node *node_ptr, PetscInt nIndex)
{
	PetscErrorCode ierr = 0;

	return ierr;
}