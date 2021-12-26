#!/usr/bin/env bash 
echo "Gshare on fp_1.bz2:"
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --gshare:13
echo "\nGshare on fp_2.bz2:"
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --gshare:13
echo "\nGshare on int_1.bz2:"
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:13
echo "\nGshare on int_2.bz2:"
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --gshare:13
echo "\nGshare on mm_1.bz2:"
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --gshare:13
echo "\nGshare on mm_2.bz2:"
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --gshare:13


echo "\n\n"
echo "\ntournament on fp_1.bz2:"
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:9:10:10
echo "\ntournament on fp_2.bz2:"
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:9:10:10
echo "\ntournament on int_1.bz2:"
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:9:10:10
echo "\ntournament on int_2.bz2:"
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:9:10:10
echo "\ntournament on fp_1.bz2:"
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:9:10:10
echo "\ntournament on mm_2.bz2:"
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:9:10:10


echo "\n\n"
echo "\ncustom on fp_1.bz2:"
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --custom:13:11:11
echo "\ncustom on fp_2.bz2:"
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --custom:13:11:11
echo "\ncustom on int_1.bz2:"
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom:13:11:11
echo "\ncustom on int_2.bz2:"
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --custom:13:11:11
echo "\ncustom on mm_1.bz2:"
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --custom:13:11:11
echo "\ncustom on mm_2.bz2:"
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --custom:13:11:11


# echo "\n\nfp_1.bz2:\n"
# echo "Gshare:"
# bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --gshare:25
# echo "\nTournament:"
# bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:21:22:22

# echo "\n\nfp_2.bz2:\n"
# echo "Gshare:"
# bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --gshare:25
# echo "\nTournament:"
# bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:21:22:22

# echo "\n\nint_1.bz2:\n"
# echo "Gshare:"
# bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:25
# echo "\nTournament:"
# bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:21:22:22

# echo "\n\nint_2.bz2:\n"
# echo "Gshare:"
# bunzip2 -kc ../traces/int_2.bz2 | ./predictor --gshare:25
# echo "\nTournament:"
# bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:21:22:22

# echo "\n\nmm_1.bz2:\n"
# echo "Gshare:"
# bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --gshare:25
# echo "\nTournament:"
# bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --tournament:21:22:22

# echo "\n\nmm_2.bz2:\n"
# echo "Gshare:"
# bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --gshare:25
# echo "\nTournament:"
# bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:21:22:22