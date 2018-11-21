
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par -max_steps 10000000 -f_tol 1e-6 -alpha 1e-3
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par -max_steps 10000000 -f_tol 1e-7 -alpha 1e-3
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par -max_steps 10000000 -f_tol 1e-8 -alpha 1e-3
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par -max_steps 10000000 -f_tol 1e-9 -alpha 1e-3
mpirun -n 1 ./model data/par/rnd2D/rnd_2d_01.par -max_steps 10000000 -f_tol 1e-10 -alpha 1e-3
