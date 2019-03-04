#!/bin/bash
#SBATCH --job-name=job1
#SBATCH --partition=WORKQ
#SBATCH --output=job1.out
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4

mpirun ./main ${1}
