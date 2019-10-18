#!/usr/bin/env bash

FLAGS="-pc_type lu -pc_factor_mat_solver_type mumps -gamma 0.01"

#for DIM in xy yz
#do
#    for SEED in 1 3 4 5 7
#    do
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm10_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm20_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm30_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm40_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm50_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm50.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm100_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm200_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm300_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm400_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm500_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm500.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm100_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm200_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm300_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm400_rt.par $FLAGS
#        mpiexec -n 1 ./model data/par/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm500_rt.par $FLAGS
#    done
#done

for DIM in xy yz
do
    for SEED in 1 3 4 5 7
    do
        # clean up previous since appending
        rm "sht3d_s0${SEED}_p${DIM}_f50_sh2.csv"
        rm "sht3d_s0${SEED}_p${DIM}_f150_sh2.csv"
        rm "sht3d_s0${SEED}_p${DIM}_f250_sh2.csv"

        declare -a arr=("data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm10_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm20_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm30_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm40_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f50_mf13_sh2_sd52_sc_cf13_cd62_cm50_rt.csv"
                        )
        for i in "${arr[@]}"
        do
            python3 tests/otherTests/merge_csv_files.py "sht3d_s0${SEED}_p${DIM}_f50_sh2.csv" "$i"
        done

        declare -a ar1=("data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm50.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm100_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm200_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm300_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm400_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f150_mf13_sh2_sd52_sc_cf13_cd62_cm500_rt.csv"
                        )
        for i in "${ar1[@]}"
        do
            python3 tests/otherTests/merge_csv_files.py "sht3d_s0${SEED}_p${DIM}_f150_sh2.csv" "$i"
        done

        declare -a ar2=("data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm500.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm100_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm200_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm300_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm400_rt.csv"
                        "data/res/sht/sht3d_s0${SEED}_p${DIM}_sz1_f250_mf13_sh2_sd52_sc_cf13_cd62_cm500_rt.csv"
                        )
        for i in "${ar2[@]}"
        do
            python3 tests/otherTests/merge_csv_files.py "sht3d_s0${SEED}_p${DIM}_f250_sh2.csv" "$i"
        done
    done
done
