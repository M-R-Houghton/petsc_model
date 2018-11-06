mpirun -n 1 ./model data/par/lmb/lmb_2d_01.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_02.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_add_01.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_add_02.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_add_03.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_add_04.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_lshft.par
1e-16 mpirun -n 1 ./model data/par/lmb/lmb_2d_rshft.par
1e-16 
mpirun -n 1 ./model data/par/lat2D/dil_0_94.par
1e-16 
#mpirun -n 1 ./model data/par/lat2D/dil_0_94.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/lat2D/dil_0_94_gm_npc.ppm
#mpirun -n 1 ./model data/par/lat2D/dil_0_94.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/lat2D/dil_0_94_gm_wjc.ppm
#mpirun -n 1 ./model data/par/lat2D/dil_0_94.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/lat2D/dil_0_94_gm_sor.ppm
#mpirun -n 1 ./model data/par/lat2D/dil_0_94.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/lat2D/dil_0_94_gm_eis.ppm
#mpirun -n 1 ./model data/par/lat2D/dil_0_94.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/lat2D/dil_0_94_gm_ilu.ppm

mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par
1e-16 mpirun -n 1 ./model data/par/rnd2D/rnd_2d_02.par
1e-16 mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_04.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_05.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_06.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_07.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_08.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_09.par
1e-16 #mpirun -n 1 ./model data/par/rnd2D/rnd_2d_10.par
1e-16 
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_01_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_01_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_01_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_01_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_01_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_02.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_02_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_02.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_02_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_02.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_02_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_02.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_02_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_02.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_02_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_03_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_03_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_03_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_03_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_03_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_04.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_04_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_04.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_04_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_04.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_04_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_04.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_04_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_04.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_04_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_05.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_05_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_05.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_05_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_05.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_05_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_05.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_05_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_05.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_05_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_06.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_06_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_06.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_06_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_06.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_06_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_06.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_06_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_06.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_06_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_07.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_07_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_07.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_07_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_07.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_07_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_07.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_07_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_07.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_07_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_08.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_08_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_08.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_08_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_08.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_08_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_08.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_08_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_08.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_08_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_09.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_09_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_09.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_09_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_09.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_09_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_09.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_09_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_09.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_09_gm_ilu.ppm

#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_10.par 1e-16 -ksp_type gmres -pc_type none -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_10_gm_npc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_10.par 1e-16 -ksp_type gmres -pc_type jacobi -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_10_gm_wjc.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_10.par 1e-16 -ksp_type gmres -pc_type sor -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_10_gm_sor.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_10.par 1e-16 -ksp_type gmres -pc_type eisenstat -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_10_gm_eis.ppm
#mpirun -n 1 ./model data/par/rnd2D/rnd_2d_10.par 1e-16 -ksp_type gmres -pc_type ilu -ksp_atol 1e-8 -ksp_monitor_lg_residualnorm -draw_save_final_image data/img/rnd2D/rnd_2d_10_gm_ilu.ppm

