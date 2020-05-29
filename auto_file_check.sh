#!/usr/bin/env bash

# If number of arguments less then 1; print usage and exit
if [ $# -lt 1 ]; then
    printf "Usage: %s <network_group>\n" "$0" >&2
    exit 1
fi

bin="model"         # The application (from command arg)
diff="diff -iadq"   # Diff command with selected flags
group="$1"          # Test group

# Set up paths
head="data/"
par_path="${head}par/${group}/"
dat_path="${head}dat/${group}/"
res_path="${head}res/${group}/"

# File containing networks to be tested
test_file="${dat_path}${group}TestNetworks.txt"

# Temporary file to store results
tmp_res_tst="temp_res.txt"

# Success counters
fail_count=0
skip_count=0
pass_count=0
total_count=0

# Check test file exists
if [ ! -f "$test_file" ]; then
    printf "Test file %s is missing\n" "$test_file"
    exit 1
fi

# Loop the array
while read file; do
    # Padd file_base with suffixes
    file_par="${par_path}${file}.par"               # The parameter file
    file_res_val="${res_path}${file}.val"           # The results file
    file_res_tst="${res_path}${file}.res"           # The results file
    file_in="${dat_path}${file}_in.dat"             # The in data file
    file_out_val="${dat_path}${file}_out.val"       # The out data to check against
    file_out_tst="${dat_path}${file}_out.dat"       # The out data from model run

    total_count=$((total_count+2))

    # Check input, parameter and out validate file exists
    if [ ! -f "$file_par" ]; then
        printf "Parameter file %s is missing\n" "$file_par"
        skip_count=$((skip_count+2))
        continue;
    fi
    if [ ! -f "$file_in" ]; then
        printf "Input data file %s is missing\n" "$file_in"
        skip_count=$((skip_count+2))
        continue;
    fi
    if [ ! -f "$file_out_val" ]; then
        printf "Output validation file %s is missing\n" "$file_out_val"
        skip_count=$((skip_count+2))
        continue;
    fi
    if [ ! -f "$file_res_val" ]; then
        printf "Results validation file %s is missing\n" "$file_res_val"
        skip_count=$((skip_count+2))
        continue;
    fi

    printf "Auto testing model with %s...\n" "$file_par"

    # Run application, redirect in file to app, and output to out file
    # NOTE: Here we can pass $file_par as arg for rank 0, but...
    # ...for all other ranks we need to redirect from /dev/null
    # ...All stdout from all ranks is collected into $tmp_res. 
    gamma="-gamma 0.02"
    mpiexec -n 1 ./$bin $file_par $gamma</dev/null > "$tmp_res_tst"

    # Pipe temporary output through grep to collect energy values
    cat $tmp_res_tst | grep -E "Gamma|Mod|Radius|Energy" > $file_res_tst

    # Check new results files exist
    if [ ! -f "$file_out_tst" ]; then
        printf "Output data file %s is missing\n" "$file_out_tst"
        skip_count=$((skip_count+2))
        continue;
    fi
    if [ ! -f "$file_res_tst" ]; then
        printf "Results data file %s is missing\n" "$file_res_tst"
        skip_count=$((skip_count+2))
        continue;
    fi

    # Execute diff
    $diff "$file_out_tst" "$file_out_val"

    # Check exit code from previous command (ie diff)
    # We need to add this to a variable else we can't print it
    # as it will be changed by the if [
    # Iff not 0 then the files differ (at least with diff)
    e_code=$?
    if [ $e_code != 0 ]; then
            printf "\e[31m[FAILED]\e[0m Output data regression test : %d\n" "$e_code"
            ((fail_count++))
    else
            printf "\e[32m[PASSED]\e[0m Output data regression test.\n"
            ((pass_count++))
    fi

    # Execute diff
    $diff "$file_res_tst" "$file_res_val"

    # Check exit code from previous command (ie diff)
    e_code=$?
    if [ $e_code != 0 ]; then
            printf "\e[31m[FAILED]\e[0m Energy regression test : %d\n" "$e_code"
            ((fail_count++))
    else
            printf "\e[32m[PASSED]\e[0m Energy regression test.\n"
            ((pass_count++))
    fi

done < $test_file

printf "Tests \e[31mfailed\e[0m:\t%d / %d\n" "$fail_count" "$total_count"
printf "Tests \e[33mskipped\e[0m:\t%d / %d\n" "$skip_count" "$total_count"
printf "Tests \e[32mpassed\e[0m:\t%d / %d\n" "$pass_count" "$total_count"

rm $tmp_res_tst

# Clean exit with status 0
exit 0
