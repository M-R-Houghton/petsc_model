
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par -max_steps 10000000 -f_tol 1e-5 -alpha 1e-4
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par -max_steps 10000000 -f_tol 1e-6 -alpha 1e-4
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par -max_steps 10000000 -f_tol 1e-7 -alpha 1e-4
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_03.par -max_steps 10000000 -f_tol 1e-8 -alpha 1e-4
