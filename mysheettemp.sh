#!/usr/bin/env bash
# Random 3D Sheet Tests

p1="ShearModu"
p2="ShearModA"
p3="EnergyT"
p4="EnergyA"
p5="EnergyT|EnergyA"
p6="Energy"
p7="Energy|Shear"

r1="1.996e-02"
r2="1.996e-01"
r3="5.9880000000000003e-03"
r4="5.9880000000000003e-04"

declare -a arr=(1.996e-06 5.988e-06 1.996e-05 5.988e-05 1.996e-04 5.988e-04 1.996e-03 5.988e-03 1.996e-02 5.988e-02)
#declare -a arr=(1.996e-06)
#for seed in 1 2 3 
for seed in 1
do
    #for pat in $p1 $p2 $p3 $p4
    for pat in $p7 
    do
        for rad in $r2
        #for rad in "${arr[@]}"
        do
            rad_alt=$rad
            #echo "Seed=${seed}, Radius=${rad}"
            #for num in 50 100 150 200 250 300 350 400 450 500 550 600
            #for num in 100 200 300 
            for num in 100 
            do
                sht=5
                # Shear Modulus Experiments
                echo "f${num}, s0${seed}, ${rad}, xy"
                python3 "data/dat/radius_rewriter.py" "data/dat/sht/sht3d_s0${seed}_pxy_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_rt_cb_in.dat" $rad $rad_alt $(($num * ${sht}))
                mpiexec -n 1 ./model data/par/sht/sht3d_s0${seed}_pxy_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_rt_cb.par -pc_type lu -pc_factor_mat_solver_type mumps | grep -E $p7
                echo "f${num}, s0${seed}, ${rad}, yz"
                python3 "data/dat/radius_rewriter.py" "data/dat/sht/sht3d_s0${seed}_pyz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_rt_cb_in.dat" $rad $rad_alt $(($num * ${sht}))
                mpiexec -n 1 ./model data/par/sht/sht3d_s0${seed}_pyz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_rt_cb.par -pc_type lu -pc_factor_mat_solver_type mumps | grep -E $p7
                echo "f${num}, s0${seed}, ${rad}, xz"
                python3 "data/dat/radius_rewriter.py" "data/dat/sht/sht3d_s0${seed}_pxz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_rt_cb_in.dat" $rad $rad_alt $(($num * ${sht}))
                mpiexec -n 1 ./model data/par/sht/sht3d_s0${seed}_pxz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_rt_cb.par -pc_type lu -pc_factor_mat_solver_type mumps | grep -E $p7

                echo "f${num}, s0${seed}, ${rad}, xy"
                python3 "data/dat/radius_rewriter.py" "data/dat/sht/sht3d_s0${seed}_pxy_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_md_rt_cb_in.dat" $rad $rad_alt $(($num * ${sht}))
                mpiexec -n 1 ./model data/par/sht/sht3d_s0${seed}_pxy_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_md_rt_cb.par -pc_type lu -pc_factor_mat_solver_type mumps | grep -E $p7
                echo "f${num}, s0${seed}, ${rad}, yz"
                python3 "data/dat/radius_rewriter.py" "data/dat/sht/sht3d_s0${seed}_pyz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_md_rt_cb_in.dat" $rad $rad_alt $(($num * ${sht}))
                mpiexec -n 1 ./model data/par/sht/sht3d_s0${seed}_pyz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_md_rt_cb.par -pc_type lu -pc_factor_mat_solver_type mumps | grep -E $p7
                echo "f${num}, s0${seed}, ${rad}, xz"
                python3 "data/dat/radius_rewriter.py" "data/dat/sht/sht3d_s0${seed}_pxz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_md_rt_cb_in.dat" $rad $rad_alt $(($num * ${sht}))
                mpiexec -n 1 ./model data/par/sht/sht3d_s0${seed}_pxz_sz1_f${num}_mf63_sh5_sd21_sc_cf63_cd31_md_rt_cb.par -pc_type lu -pc_factor_mat_solver_type mumps | grep -E $p7

            done
        done
    done
done

