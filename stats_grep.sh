#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
    echo "[ERROR] Needs exactly one argument."
    exit 1
fi

echo "U_vec = " > stats_grepped.txt
grep -A8 "solver" $1 | grep -vE "solver|seq|Vec" >> stats_grepped.txt
grep -E "Energy|E_" $1 >> stats_grepped.txt

