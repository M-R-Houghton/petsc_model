mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-1 -ts_restart false
mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-2 -ts_restart false
mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-3 -ts_restart false
mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-4 -ts_restart false
mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-5 -ts_restart false
mpirun -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -use_ksp false -use_ts true -alpha 1e-6 -ts_restart false
