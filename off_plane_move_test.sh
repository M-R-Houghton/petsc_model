#!/usr/bin/env bash

# make backup of original lmb file
cp data/dat/lmb/lmb_3d_p01_in.dat data/dat/lmb/lmb_3d_p01_in.dat.bak

# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.75/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma 0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.70/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma 0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.65/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma 0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.60/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma 0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.55/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma 0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.50/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma 0.01

# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.75/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma -0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.70/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma -0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.65/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma -0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.60/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma -0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.55/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma -0.01
# change z value
sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.50/g" data/dat/lmb/lmb_3d_p01_in.dat
# test for this z value
mpirun -n 1 ./model data/par/lmb/lmb_3d_p01.par -gamma -0.01

# reset to backup of original lmb file
cp data/dat/lmb/lmb_3d_p01_in.dat.bak data/dat/lmb/lmb_3d_p01_in.dat

# clean up
rm data/dat/lmb/lmb_3d_p01_in.dat.bak
rm data/dat/lmb/lmb_3d_p01_in.dat.tmp
