for (( c=1; c<=3; c++))
do
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_f600_in.dat" "../../data/log/rnd_3d_s0"$c"_f600.log" 
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_f800_in.dat" "../../data/log/rnd_3d_s0"$c"_f800.log"
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_f1000_in.dat" "../../data/log/rnd_3d_s0"$c"_f1000.log"

	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_lf600_in.dat" "../../data/log/rnd_3d_s0"$c"_lf600.log" 
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_lf800_in.dat" "../../data/log/rnd_3d_s0"$c"_lf800.log"
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_lf1000_in.dat" "../../data/log/rnd_3d_s0"$c"_lf1000.log"

	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_f600_apxz_in.dat" "../../data/log/rnd_3d_s0"$c"_f600_apxz.log" 
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_f800_apxz_in.dat" "../../data/log/rnd_3d_s0"$c"_f800_apxz.log"
	python3 network_general_analysis.py "../../data/dat/rnd3D/rnd_3d_s0"$c"_f1000_apxz_in.dat" "../../data/log/rnd_3d_s0"$c"_f1000_apxz.log"
done