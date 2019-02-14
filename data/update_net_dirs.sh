#!/usr/bin/env bash

if [[ ! ./ -ef ~/Documents/university/postgrad/optimisingRFN/petsc_model/data ]]; then
    echo "[ERROR] Running from incorrect directory"
    exit 0
fi

find par -type d > net_dirs.txt
