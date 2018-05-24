#include "networkAnalysisTests.h"

int test_networkAnalysis()
{
	check(networkAnalysis() == 0, "networkAnalysis failed");
	return 0;

error:
	return -1;
}