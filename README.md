# Numerical Model for Elastic Fibre Networks

## Quick Start
Compile the model using `make`, which results in the executable `model`. 
Each network data file must be passed to the model via a parameter file, which can be generated automatically using

```
make parfiles
```

This command will look in each of the existing subdirectories `subdir` of `data/dat/subdir` and build parameter files for all of the network data files with a filename ending in `_in.dat`. To run the model use

```
mpiexec -n 1 ./model data/par/parameterFile.par [optional_args]
```

This command runs the model in serial, for the network corresponding with the relevant parameter file found at `data/par/parameterFile.par`.

## Further Information 
For additional information and model documentation please see the project [wiki](https://github.com/M-R-Houghton/petsc_model/wiki)

