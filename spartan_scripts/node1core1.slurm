#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node 1
#SBATCH --cpus-per-task 1
#SBATCH --job-name=nodes1_cores1
#SBATCH --time=10:00
#SBATCH --partition=physical
#SBATCH --output=results/nodes1_cores1.txt

#Specify your email address to be notified of progress.
#Includes job usage statistics
#SBATCH --mail-user=changwenl@student.unimelb.edu.au
#SBATCH --mail-type=ALL

module load gcc/11.2.0
module load openmpi/4.1.1
mpirun -np 1 app

##Log this job's resource usage stats###
my-job-stats -a -n -s
##