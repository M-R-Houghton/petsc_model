#!/bin/bash

module load intel/17.0.1 mkl/2017.1
module load cmake

unset CC CXX FC F77 F90

./configure \
PETSC_DIR=`pwd` \
PETSC_ARCH='intel_intelmpi_mkl-17.1' \
COPTFLAGS='-O3 -mfpmath=sse -xCORE-AVX2' \
CXXOPTFLAGS='-O3 -mfpmath=sse -xCORE-AVX2' \
--with-mpi-dir=/apps/developers/libraries/intelmpi/2017.1.132/1/intel-17.0.1 \
--download-superlu --download-mumps \
--with-gnu-compilers=0 \
--with-vendor-compilers=1 \
--with-pic \
--with-shared-libraries=1 \
--with-c-support \
--with-fortran \
--with-zlib=1 \
--with-scalapack-lib="-lmkl_scalapack_lp64 -lmkl_blacs_intelmpi_lp64 -liomp5 -lpthread -lm -ldl" \
--with-blaslapack-lib="-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl" \

#CC=icc \
#CXX=icpc \
#FC=ifort \
#F77=ifort \
#F90=ifort \
