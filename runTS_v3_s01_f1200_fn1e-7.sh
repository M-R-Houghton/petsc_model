# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=24:00:00
# Set memory required
#$ -l h_vmem=4G
# Send email at start and end
#$ -m be
# run petsc using command file
#make -f makefile.arc3

mpiexec -n 1 ./model data/par/rnd3D/rnd_3d_v3_s01_f1200.par -max_steps 100000000 -f_tol 1e-7 -alpha 1e-4
cp data/dat/rnd3D/rnd_3d_v3_s01_f1200_out.dat data/dat/rnd3D/rnd_3d_v3_s01_f1200_fn1e-7_out.dat
