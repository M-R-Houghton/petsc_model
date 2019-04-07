

mpirun -n 1 ./model data/par/tri/tri_3d_01.par -gamma 0.02 >> tmp.out
rm tmp.out
diff data/dat/ref/f3tTripod1_out.dat data/dat/tri/tri_3d_01_out.dat
if [ $? -ne 0 ]; then
    echo "[ ERROR ] Inconsistent files";
fi
