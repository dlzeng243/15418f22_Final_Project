#!/bin/sh

# easy
eval ./script --height 2 --width  5 --seed 1 > input_random/easy/easy1.txt
eval ./script --height 3 --width  6 --seed 2 > input_random/easy/easy2.txt
eval ./script --height 5 --width  5 --seed 3 > input_random/easy/easy3.txt
eval ./script --height 2 --width 10 --seed 4 > input_random/easy/easy4.txt
eval ./script --height 4 --width  6 --seed 5 > input_random/easy/easy5.txt

# medium
eval ./script --height 4 --width 10 --seed  6 > input_random/medium/medium1.txt
eval ./script --height 7 --width  7 --seed  7 > input_random/medium/medium2.txt
eval ./script --height 8 --width  8 --seed  8 > input_random/medium/medium3.txt
eval ./script --height 6 --width 10 --seed  9 > input_random/medium/medium4.txt
eval ./script --height 4 --width 15 --seed 10 > input_random/medium/medium5.txt
# hard
eval ./script --height 10 --width 10 --seed 11 > input_random/hard/hard1.txt
eval ./script --height 10 --width 20 --seed 12 > input_random/hard/hard2.txt
eval ./script --height 12 --width 15 --seed 13 > input_random/hard/hard3.txt
eval ./script --height 13 --width 12 --seed 14 > input_random/hard/hard4.txt
eval ./script --height  5 --width 30 --seed 15 > input_random/hard/hard5.txt
# extreme
eval ./script --height 20 --width 20 --seed 16 > input_random/extreme/extreme1.txt
eval ./script --height 10 --width 40 --seed 17 > input_random/extreme/extreme2.txt
eval ./script --height 16 --width 16 --seed 18 > input_random/extreme/extreme3.txt
eval ./script --height 12 --width 24 --seed 19 > input_random/extreme/extreme4.txt
eval ./script --height  8 --width 36 --seed 20 > input_random/extreme/extreme5.txt