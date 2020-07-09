#!/usr/bin/env bash

if mpiexec -n 1 ./model data/par/lat2D/lat2d_60_s01.par> /dev/null 2>&1; then
    echo "program succeeded"
else
    echo "program failed"
fi

until mpiexec -n 1 ./model data/par/lat2D/lat2d_60_s01.par> /dev/null 2>&1; do
    if [ $? -eq 0 ]; then
        echo "Program success"
    else
        echo "Error was not 5"
    fi
    # potentially, other code follows...
done
