# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=1:00:00
# Set memory required
#$ -l h_vmem=16G
# Send email at start and end
#$ -m be
# run petsc using command file
#cp data/sol/sol.f3tRnd_s01_f800_t5_jcb
#make clean && make
#mpiexec -n 1 ./model -ksp_type gmres -pc_type bjacobi -pc_bjacobi_blocks 9 -sub_ksp_type preonly -sub_pc_type lu
mpiexec -n 1 ./model data/par/tri/tri_3d_01.par -log_view