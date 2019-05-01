#!/usr/bin/env bash

MYSTRING=$1
FILENAME=$2

grep $MYSTRING $FILENAME | grep -o "[0-9].*" | tr '\n' ','
echo ""
