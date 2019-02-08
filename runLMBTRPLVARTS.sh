# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=2:00:00
# Set memory required
#$ -l h_vmem=4G
# Send email at start and end
#$ -m be
# run petsc using command file
#make clean && make

mpiexec -n 1 ./model data/par/lmb/lmb_3d_trpl_var.par -use_ksp false -use_ts true -alpha 1e3
mpiexec -n 1 ./model data/par/lmb/lmb_3d_trpl_var.par -use_ksp false -use_ts true -alpha 1e2
mpiexec -n 1 ./model data/par/lmb/lmb_3d_trpl_var.par -use_ksp false -use_ts true -alpha 1e1
mpiexec -n 1 ./model data/par/lmb/lmb_3d_trpl_var.par -use_ksp false -use_ts true -alpha 1e0
mpiexec -n 1 ./model data/par/lmb/lmb_3d_trpl_var.par -use_ksp false -use_ts true -alpha 1e-1
mpiexec -n 1 ./model data/par/lmb/lmb_3d_trpl_var.par -use_ksp false -use_ts true -alpha 1e-2

