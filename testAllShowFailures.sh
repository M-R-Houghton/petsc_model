#!/bin/bash

TESTS_EXE="localAssembly localAssemblyStretch localAssemblyBend
			globalAssemblyStretch globalAssemblyBend networkBuild
			networkDestroy networkRead networkWrite
			vectorOperations systemAssembly systemSolve
			networkUpdate networkValidation parameterRead"

make clean
make

cd tests/gUnitTests

make gclean
make all

if [ "$#" -ne 1 ]; then

	rm unitTestResults.o
	rm unitTestResults.e

    for test in $TESTS_EXE
    do
    	echo $test
		mpirun -n 1 ./$test >> unitTestResults.o
	done

	python3 unitTestFailureParser.py unitTestResults.o
	cat unitTestResults.e

    cd ../../
    exit 1
fi

./$1
cd ../../
