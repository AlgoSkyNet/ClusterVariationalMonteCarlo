for f in *.out; do
echo $f
(./fitEcho.sh $f | gnuplot 2>&1 ) | grep -A 5 "Final" | tail -n 3
done
