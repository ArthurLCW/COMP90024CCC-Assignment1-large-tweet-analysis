#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node 8
#SBATCH --cpus-per-task 1
#SBATCH --job-name=nodes1_cores8
#SBATCH --time=3:00
#SBATCH --partition=physical
#SBATCH --output=results/nodes1_cores8.txt

#Specify your email address to be notified of progress.
#Includes job usage statistics
#SBATCH --mail-user=changwenl@student.unimelb.edu.au
#SBATCH --mail-type=ALL

module load gcc/11.2.0
module load openmpi/4.1.1
mpirun -np 8 app

##Log this job's resource usage stats###
my-job-stats -a -n -s
##