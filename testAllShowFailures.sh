#!/bin/bash

make clean
make

cd tests/gUnitTests

make gclean
make all

if [ "$#" -ne 1 ]; then

	rm unitTestResults.o
	rm unitTestResults.e

    for test in *_unittest
    do
    	echo $test
		./$test >> unitTestResults.o
	done

	python3 unitTestFailureParser.py unitTestResults.o
	cat unitTestResults.e

    cd ../../
    exit 1
fi

./$1
cd ../../
