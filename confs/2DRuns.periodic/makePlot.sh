for f in *.tpl; do
   for U in `seq 0 16`; do
      MU=`echo $U/2.0 | bc -l`;
      nf=`echo $f | sed s/"_U_"/_"$U"_/ | sed s/"_MU_"/_"$MU"_/ | sed s/"tpl"/"conf"/`;
      echo $nf
      
   done;
done 
