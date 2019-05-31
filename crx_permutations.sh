#!/usr/bin/env bash

# make backup of original lmb file
cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat.bak

dz="3"
qz="4"
cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 3 2 0.7 1.0 \).*/\10.$dz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 5 0 0.5 0.5 \).*/\10.$qz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'

dz="4"
qz="45"
cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 3 2 0.7 1.0 \).*/\10.$dz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 5 0 0.5 0.5 \).*/\10.$qz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'

dz="5"
qz="5"
cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 3 2 0.7 1.0 \).*/\10.$dz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 5 0 0.5 0.5 \).*/\10.$qz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'

dz="6"
qz="55"
cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 3 2 0.7 1.0 \).*/\10.$dz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 5 0 0.5 0.5 \).*/\10.$qz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'

dz="7"
qz="6"
cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 3 2 0.7 1.0 \).*/\10.$dz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'
sed -i .tmp "s/\(n 5 0 0.5 0.5 \).*/\10.$qz/g" data/dat/crx/crx_cz5_dz$dz'_mdm_clf_in.dat'

#cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz4_mdm_clf_in.dat
#cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz5_mdm_clf_in.dat
#cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz6_mdm_clf_in.dat
#cp data/dat/crx/crx_cz5_dz2_mdm_clf_in.dat data/dat/crx/crx_cz5_dz7_mdm_clf_in.dat

#echo "gamma = 0.01"
#echo "z = 0.75"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.75/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma 0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.70"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.70/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma 0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.65"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.65/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma 0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.60"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.60/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma 0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.55"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.55/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma 0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.50"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.50/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma 0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#
#echo "gamma = -0.01"
#echo "z = 0.75"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.75/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma -0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.70"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.70/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma -0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.65"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.65/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma -0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.60"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.60/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma -0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.55"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.55/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma -0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#echo "z = 0.50"
## change z value
#sed -i .tmp "s/\(n 0 2 1.0 0.0 \).*/\10.50/g" data/dat/lmb/lmb_3d_ofp1_in.dat
## test for this z value
#mpirun -n 1 ./model data/par/lmb/lmb_3d_ofp1.par -gamma -0.01 > model.out
## grep important values
#./stats_grep.sh model.out
#cat stats_grepped.txt
#
# reset to backup of original lmb file
#cp data/dat/lmb/lmb_3d_ofp1_in.dat.bak data/dat/lmb/lmb_3d_ofp1_in.dat

# clean up
#rm model.out
#rm data/dat/lmb/lmb_3d_ofp1_in.dat.bak
#rm data/dat/lmb/lmb_3d_ofp1_in.dat.tmp
