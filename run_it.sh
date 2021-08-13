PROGRAM=./interpolation

INPUT_FILE=input.txt

LIKWID_OUTPUT=likwid_out.csv

TMP_FILES=( inter_trian.tmp adjus_coef.tmp adjus_trian.tmp )

N_LIST="10 32 50 64 100 128 200 256 300 400 512 1000"

echo "inter_trian" > inter_trian.tmp
echo "adjus_coef"  > adjus_coef.tmp
echo "adjus_trian" > adjus_trian.tmp


for file in ${TMP_FILES[*]};
do
    echo "N:Memory bandwidth [MBytes/s]:Time elapsed [s]:Cache miss:DP MFLOP/s:AVX DP MFLOP/s" >> $file
done

for N in $N_LIST;
do
    python3 gera_entrada.py $N > $INPUT_FILE

    likwid-perfctr -C 3 -g L3 -o $LIKWID_OUTPUT -m $PROGRAM < $INPUT_FILE > /dev/null

    RESULT=$(cat $LIKWID_OUTPUT | grep "L3 bandwidth"  | cut -d',' -f2)
    let counter=0
    for r in $RESULT;
    do
        echo -n "$N:$r" >> ${TMP_FILES[$counter]}
        let counter++
    done
    
    RESULT=$(cat $LIKWID_OUTPUT | grep "RDTSC Runtime" | cut -d',' -f2)
    let counter=0
    for r in $RESULT;
    do
        echo -n ":$r" >> ${TMP_FILES[$counter]}
        let counter++
    done

    likwid-perfctr -C 3 -g L2CACHE -o $LIKWID_OUTPUT -m $PROGRAM < $INPUT_FILE > /dev/null

    RESULT=$(cat $LIKWID_OUTPUT | grep "L2 miss ratio" | cut -d',' -f2)
    let counter=0
    for r in $RESULT;
    do
        echo -n ":$r" >> ${TMP_FILES[$counter]}
        let counter++
    done

    likwid-perfctr -C 3 -g FLOPS_DP -o $LIKWID_OUTPUT -m $PROGRAM < $INPUT_FILE > /dev/null
    
    RESULT_FLT=$(cat $LIKWID_OUTPUT | grep -v "AVX" | grep "DP MFLOP/s" | cut -d',' -f2)
    let counter=0
    for r in $RESULT_FLT;
    do
        echo -n ":$r" >> ${TMP_FILES[$counter]}
        let counter++
    done

    RESULT_AVX=$(cat $LIKWID_OUTPUT | grep "AVX" | cut -d',' -f2)
    let counter=0
    for r in $RESULT_AVX;
    do
        echo ":$r" >> ${TMP_FILES[$counter]}
        let counter++
    done
done

for file in ${TMP_FILES[*]};
do
    echo >> $file
    column -s : -t $file
    rm $file
done


rm -rf $INPUT_FILE $AUX_FILE $LIKWID_OUTPUT