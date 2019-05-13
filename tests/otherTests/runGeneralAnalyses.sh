# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=1:00:00
# Set memory required
#$ -l h_vmem=16G
# Send email at start and end
#$ -m be

#./generateLogsForRandom.sh
python node_displacement_checker.py ../../data/dat/cpl3D/cpl_3d_mn1e-6_mx1e-2_lc2e-3_s33_f2400 
