# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=6:00:00
# Set memory required
#$ -l h_vmem=16G
# Send email at start and end
#$ -m be
# run petsc using command file
#make clean && make

#mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_s03_f600.par
mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_s03_f600.par -ksp_type gmres -ksp_gmres_restart 100 -pc_type jacobi 
#mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_s03_f800.par
mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_s03_f800.par -ksp_type gmres -ksp_gmres_restart 100 -pc_type jacobi 
#mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_s03_f1000.par
mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_s03_f1000.par -ksp_type gmres -ksp_gmres_restart 100 -pc_type jacobi 
