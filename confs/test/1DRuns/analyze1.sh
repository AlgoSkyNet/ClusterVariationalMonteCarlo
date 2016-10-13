for f in 2x1_16x1_0_0_OBC.conf.out 4x1_16x1_0_0_OBC.conf.out 8x1_16x1_0_0_OBC.conf.out; do 
./findMin.sh $f
done

for f in 2x1_16x1_2_1.00000000000000000000_OBC.conf.out 4x1_16x1_2_1.00000000000000000000_OBC.conf.out 8x1_16x1_2_1.00000000000000000000_OBC.conf.out; do
./findMin.sh $f
done

for f in 2x1_16x1_4_2.00000000000000000000_OBC.conf.out 4x1_16x1_4_2.00000000000000000000_OBC.conf.out 8x1_16x1_4_2.00000000000000000000_OBC.conf.out; do 
./findMin.sh $f
done

for f in 2x1_16x1_8_4.00000000000000000000_OBC.conf.out 4x1_16x1_8_4.00000000000000000000_OBC.conf.out 8x1_16x1_8_4.00000000000000000000_OBC.conf.out; do
./findMin.sh $f
done

for f in 2x1_16x1_16_8.00000000000000000000_OBC.conf.out 4x1_16x1_16_8.00000000000000000000_OBC.conf.out 8x1_16x1_16_8.00000000000000000000_OBC.conf.out; do
./findMin.sh $f
done
