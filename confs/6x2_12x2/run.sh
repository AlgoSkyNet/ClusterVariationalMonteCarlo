export OMP_NUM_THREADS=4
export KMP_AFFINITY=scatter
for f in *.conf; do 
    ../../dist/Debug/GNU-Linux-x86/clustervariationalmontecarlo2 $f;
done
