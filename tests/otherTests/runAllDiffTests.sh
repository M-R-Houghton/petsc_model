#!/bin/bash

cd ../../

tri_tests=$(ls data/par/tri/tri_3d_cv*.par)

#make clean && make

echo 'TESTING TRIPOD CASES...'
mpirun -n 1 ./model data/par/tri/tri_3d_01.par | tail -17 | head -11 > tmp_base.txt

for t in $tri_tests
do
    mpirun -n 1 ./model $t | tail -17 | head -11 > tmp_test.txt
    diff tmp_base.txt tmp_test.txt | python3 tests/otherTests/diff_output_parser.py
done

cd tests/otherTests/
