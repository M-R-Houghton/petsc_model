for (( c=1; c<=4; c++))
do
	mv  "tri/f3tTripod"$c"_in.dat" "tri/tri_3d_0"$c"_in.dat"
	mv  "tri/f3tLoose"$c"_in.dat" "tri/tri_3d_add_0"$c"_in.dat"
	mv  "lmb/lmbAdd"$c"_in.dat" "lmb/lmb_2d_add_0"$c"_in.dat"
done