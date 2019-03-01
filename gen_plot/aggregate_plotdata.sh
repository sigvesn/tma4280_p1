echo "Aggregating ..."

rm ./plotdata/aggregate*.txt

for f in ./plotdata/*.out; do
    n=$(sed -n 's/.*n = \([0-9]*\).*/\1/p'                         ${f})
    p=$(sed -n 's/.*running with \([0-9]*\).*/\1/p'                ${f})
    e=$(sed -n 's/.*error=\([0-9].[0-9]*e[-|+][0-9]*\).*/\1/p'     ${f})
    d=$(sed -n 's/.*duration=\([0-9].[0-9]*e[-|+][0-9]*\).*/\1/p'  ${f})

    if ! [ -z "${n}" ] && ! [ -z "${e}" ] && ! [ -z "${d}" ]; then
        echo $n $e $d >> ./plotdata/aggregate_p${p}.txt
    fi
    
done


