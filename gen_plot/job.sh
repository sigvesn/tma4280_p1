#!/bin/bash
#SBATCH --job-name=JOB
#SBATCH --partition=WORKQ
#SBATCH --output=./plotdata/OUT
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=TASKS

mpirun ./main NUMBER
