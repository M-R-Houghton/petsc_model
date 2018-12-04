for (( c=1; c<=3; c++))
do
	#mv "rnd3D/rnd_3d_s0"$c"_lf600_out.dat" "rnd3D/rnd_3d_s0"$c"_lf600_lu_out.dat"
	#mv "rnd3D/rnd_3d_s0"$c"_lf800_out.dat" "rnd3D/rnd_3d_s0"$c"_lf800_lu_out.dat"
	mv "rnd3D/rnd_3d_s0"$c"_lf1000_out.dat" "rnd3D/rnd_3d_s0"$c"_lf1000_lu_out.dat"
done