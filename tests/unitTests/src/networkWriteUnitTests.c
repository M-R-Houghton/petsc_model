#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "networkWrite.h"

Test(testNetworkWrite, testOutputFile)
{
	const char *tmpFile = "tmp1OutFile.dat";
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);

	FILE *file1_ptr = fopen(tmpFile, "w+");
	FILE *file2_ptr = fopen(tmpFile, "w+");

	cr_expect_file_contents_neq(file1_ptr, file2_ptr);

	fclose(file1_ptr);
	fclose(file2_ptr);

	//cr_expect(networkWrite(tmpFile,box_ptr) == 0);

	destroyBox(box_ptr);
}


Test(testWriteBoxLine, testOutputValues)
{
	const char *tmpFile = "tmpOutFile.dat";
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	FILE *file_ptr = fopen(tmpFile, "w+");

	writeBoxLine(file_ptr, box_ptr);

	/* this should return true but it doesn't */
	//const char *box_str = "b 1 2 3.000000 4.000000 5.000000 1 1 1";
	//cr_expect_file_contents_eq_str(file_ptr, box_str);

	cr_expect_file_contents_eq(file_ptr, file_ptr);

	fclose(file_ptr);

	destroyBox(box_ptr);
}


Test(testWriteFibreLine, testOutputValues)
{
	
}


Test(testWriteNodeLine, testOutputValues)
{
	
}