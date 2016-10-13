for f in *.tpl; do
   echo $f
   for U in `seq 0 16`; do
      MU=`echo $U/2.0 | bc -l`;
      nf=`echo $f | sed s/"_U_"/_"$U"_/ | sed s/"_MU_"/_"$MU"_/ | sed s/"tpl"/"conf"/`.out;
      nold=0.0
      for n in `cut -d' ' -f3 $nf`; do
         if [ `echo "$n < $nold" | bc -l ` = 1 ]; then nold=$n; fi
      done
      nold=`echo $nold+$MU |bc -l`
      echo $U $nold      
   done;
   echo
done 
