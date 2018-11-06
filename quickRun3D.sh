mpirun -n 1 ./model data/par/tri/tri_3d_01.par 1e-16
mpirun -n 1 ./model data/par/tri/tri_3d_02.par 1e-16
mpirun -n 1 ./model data/par/tri/tri_3d_03.par 1e-16
mpirun -n 1 ./model data/par/tri/tri_3d_04.par 1e-16

mpirun -n 1 ./model data/par/tri/tri_3d_add_01.par 1e-16
mpirun -n 1 ./model data/par/tri/tri_3d_add_02.par 1e-16
mpirun -n 1 ./model data/par/tri/tri_3d_add_03.par 1e-16
mpirun -n 1 ./model data/par/tri/tri_3d_add_04.par 1e-16

mpirun -n 1 ./model data/par/lat3D/lat_3d_02.par 1e-16

#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_s01_f600.par 1e-16
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_s01_f600_nt.par 1e-16
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_s01_f800.par 1e-16
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_s01_f800_nt.par 1e-16

#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 10 -pc_type none -ksp_atol 1e-8 -ksp_monitor_true_residual
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 10 -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_true_residual
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 10 -pc_type sor -ksp_atol 1e-8 -ksp_monitor_true_residual
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 10 -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_true_residual

#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 1000 -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd3D/rnd_3d_v3_s01_f1000_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 1000 -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd3D/rnd_3d_v3_s01_f1000_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 1000 -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd3D/rnd_3d_v3_s01_f1000_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1000.par 1e-16 -ksp_type gmres -ksp_max_it 1000 -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd3D/rnd_3d_v3_s01_f1000_gm_ilu.ppm