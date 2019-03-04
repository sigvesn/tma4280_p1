#!/bin/bash
# Uses the job.sh template configure and run a sbatch job for the './main'
# program where this script is run from for the following parameters:
#   processors = 1,2,4,8
#   n = 2^k, for k = 1,2,...,24
# All results are put in the plotdata folder

for i in $(seq 1 24); do
    ns[i]=$(echo 2^$i | bc)
done

mkdir -p plot
mkdir -p plotdata

for f in plotdata/*; do
    rm $f
done

C_DIR=$(dirname $PWD)
CURRENT=${C_DIR##*/)}
TYPE=${CURRENT##*[a-z]}

if [ $TYPE -eq 2 ]; then
    echo Generation plotdata for mpi implementation ...

    DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
    JOB=$DIR/job.sh

    # each job script is setup with two nodes per task
    for i in $(seq 1 4); do
        ps[i]=$(echo 2^$i / 2 | bc)
    done

    for n in ${ns[*]}; do
        for p in ${ps[*]}; do
            name="job_n${n}_p${p}.sh"
            cp $JOB plot/$name

            sed -i "s/JOB/$name/g"          plot/$name
            sed -i "s/OUT/${n}_p${p}.out/g" plot/$name
            sed -i "s/TASKS/$p/g"           plot/$name
            sed -i "s/NUMBER/$n/g"          plot/$name

            sbatch plot/$name
        done
    done

elif [ $TYPE -eq 3 ]; then
    echo Generation plotdata for omp implementation ...

    for i in $(seq 1 4); do
        ps[i]=$(echo 2^$i | bc)
    done

    for n in ${ns[*]}; do
        for p in ${ps[*]}; do
            name="job_n${n}_p${p}.sh"
            export OMP_NUM_THREADS=${p}
            ./main ${n} >> plotdata/${n}_p${p}.out
        done
    done

else
    echo "Must be run from [zeta|mach][2|3]"
fi
