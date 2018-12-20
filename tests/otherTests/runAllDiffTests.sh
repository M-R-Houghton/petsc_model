#!/bin/bash

cd ../../

#make clean && make

echo 'TESTING LAMBDA CASES (3D PLANAR)...'
# run base case
mpirun -n 1 ./model data/par/tri/tri_3d_p01.par | tail -17 | head -11 > tmp_base.txt

# run tests to compare against base
tri_tests=$(ls data/par/lmb/lmb_3d_cv*.par)
for t in $tri_tests
do
    mpirun -n 1 ./model $t | tail -17 | head -11 > tmp_test.txt
    diff tmp_base.txt tmp_test.txt | python3 tests/otherTests/diff_output_parser.py
done

echo 'TESTING TRIPOD CASES...'
# run base case
mpirun -n 1 ./model data/par/tri/tri_3d_01.par | tail -17 | head -11 > tmp_base.txt

# run tests to compare against base
tri_tests=$(ls data/par/tri/tri_3d_cv*.par)
for t in $tri_tests
do
    mpirun -n 1 ./model $t | tail -17 | head -11 > tmp_test.txt
    diff tmp_base.txt tmp_test.txt | python3 tests/otherTests/diff_output_parser.py
done

# clean up 
rm tmp_base.txt
rm tmp_test.txt

cd tests/otherTests/
