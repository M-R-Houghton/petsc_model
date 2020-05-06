#!/usr/bin/env bash

if mpiexec -n 1 ./model data/par/lat2D/lat2d_60_s03.par> /dev/null 2>&1; then
    echo "program succeeded"
else
    echo "program failed"
fi
