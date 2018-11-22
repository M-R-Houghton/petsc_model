# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=4:00:00
# Set memory required
#$ -l h_vmem=8G
# Send email at start and end
#$ -m be
# run petsc using command file
#make -f makefile.arc3

mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -max_steps 100000000 -f_tol 1e-5 -alpha 1e-2
cp data/dat/rnd3D/rnd_3d_v3_s01_f1200_out.dat data/dat/rnd3D/rnd_3d_v3_s01_f1200_fn1e-5_out.dat
