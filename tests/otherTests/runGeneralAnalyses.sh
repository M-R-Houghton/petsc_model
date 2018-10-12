# Export current environment
#$ -cwd -V
# Set a 10 min limit
#$ -l h_rt=1:00:00
# Set memory required
#$ -l h_vmem=16G
# Send email at start and end
#$ -m be

./generateLogsForRandom.sh
