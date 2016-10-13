export OMP_NUM_THREADS=4
export KMP_AFFINITY=scatter
for f in *.conf; do 
   /home/chris/NetBeansProjects/ClusterVariationalMonteCarlo/dist/Debug/GNU-Linux-x86/clustervariationalmontecarlo $f;
   #./cvmc $f;
done
