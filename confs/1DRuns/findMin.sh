   f=$1
      nf=$f
      nold=0.0
      for n in `cut -d' ' -f3 $nf`; do
         if [ `echo "$n < $nold" | bc -l ` = 1 ]; then nold=$n; fi
         #echo $n $nold
      done
      echo $nold      
