for i in $(seq 1 24); do
    ns[i]=$(echo 2^$i | bc)
done

ps[0]=1; ps[1]=2; ps[2]=4; ps[3]=8

DIR="$(cd "$( dirname "${BASH_SOURCE[0]}"  )" >/dev/null 2>&1 && pwd)"
JOB=$DIR/job.sh

mkdir -p plot
mkdir -p plotdata

for n in ${ns[*]}; do
    for p in ${ps[*]}; do
        echo n: $n, p: $p
        name="job_n${n}_p${p}.sh"
        cp $JOB ./plot/$name
        sed -i "s/JOB/$name/g"          ./plot/$name
        sed -i "s/OUT/${n}_p${p}.out/g" ./plot/$name
        sed -i "s/TASKS/$p/g"           ./plot/$name
        sed -i "s/NUMBER/$n/g"          ./plot/$name

        sbatch ./plot/$name
    done
done
