#include <petscksp.h>
#include <criterion/criterion.h>
#include "networkWrite.h"

Test(testNetworkWrite, testOutputFile)
{
	const char *tmpFile = "tmpOutFile.dat";
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);

	//cr_expect(networkWrite(tmpFile,box_ptr) == 0);
}


Test(testWriteBoxLine, testOutputValues)
{
	const char *tmpFile = "tmpOutFile.dat";
	Box *box_ptr = makeBox(1,2,3,4,5,1,1,1);
	FILE *file_ptr = fopen(tmpFile, "w+");

	writeBoxLine(file_ptr, box_ptr);

	fclose(file_ptr);
	destroyBox(box_ptr);
}


Test(testWriteFibreLine, testOutputValues)
{
	
}


Test(testWriteNodeLine, testOutputValues)
{
	
}