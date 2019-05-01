#!/usr/bin/env bash

FILENAME=$1
GREPFILE=$2

./propertyParse.sh "EnergyStre" $GREPFILE >> $FILENAME
./propertyParse.sh "EnergyBend" $GREPFILE >> $FILENAME
./propertyParse.sh "EnergyTotl" $GREPFILE >> $FILENAME
./propertyParse.sh "EnergyAffn" $GREPFILE >> $FILENAME
./propertyParse.sh "ShearModulus" $GREPFILE>> $FILENAME
./propertyParse.sh "ShearModAffn" $GREPFILE>> $FILENAME
