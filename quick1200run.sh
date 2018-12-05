mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e1 -f_tol 1e-12 -ts_restart true 
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-2 -f_tol 1e-12 -ts_restart false 
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-3 -f_tol 1e-12 -ts_restart false 
#mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-4 -f_tol 1e-12 -ts_restart false 
