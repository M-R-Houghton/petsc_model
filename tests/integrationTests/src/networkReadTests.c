#include "networkReadTests.h"

const char *test_dataRead() 
{
    int foo = 7;
    mu_assert(foo == 7, "error, foo != 7");
    return 0;
}

const char *all_data() 
{
    mu_run_test(test_dataRead);
    return 0;
}

/* Tests network read in routine */
PetscErrorCode test_networkRead()
{
	PetscErrorCode ierr;
	Box *box_ptr;

    const char *fileToRead = "data/dat/tri/tri_3d_01_in.dat";

	ierr = networkRead(fileToRead, &box_ptr, 0.05);CHKERRQ(ierr);

	assert(test_dataRead() == 0);

	/* test open file */

	/* test read in line by line */

		/* test process box line */

		/* test process fibre line */
		/* test make fibre */

		/* test process node line */
		/* test make node */

	/* test close file */

	destroyBox(box_ptr);

	return ierr;
}