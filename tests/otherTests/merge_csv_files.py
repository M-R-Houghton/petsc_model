#!/usr/bin/env python3
# merge_csv_files.py

# Imports
import sys
import csv

def check_csv_headers(filename, row):

    print("[STATUS]\t Checking csv headers of " + filename + "...")

    assert row[0]  == "Dimension"
    assert row[1]  == "Periodicity"
    assert row[2]  == "TotalNodes"
    assert row[3]  == "InternalNodes"
    assert row[4]  == "TotalFibres"
    assert row[5]  == "TotalCouples"
    assert row[6]  == "Gamma"
    assert row[7]  == "YoungsModulus"
    assert row[8]  == "Radius"
    assert row[9]  == "EnergyStre"
    assert row[10] == "EnergyBend"
    assert row[11] == "EnergyTotl"
    assert row[12] == "EnergyPsAf"
    assert row[13] == "EnergyAffn"
    assert row[14] == "ShearModulus"
    assert row[15] == "ShearModAffn"
    
    print("[STATUS]\t ...success.")

    return 1


if __name__ == '__main__':

    # Check merge_csv_files.py has a valid number of arguments
    if len(sys.argv) < 2:
        sys.exit('Usage: merge_csv_files.py <new_file.csv> <file1.csv> <file2.csv>')

    new_filename = sys.argv[1]

    print("[STATUS] Collecting csv files to merge...")
    filename_list = []
    for i in range(2, len(sys.argv)):
        filename_list.append(sys.argv[i])
        print("[STATUS]\t"+str(sys.argv[i]))
    print("[STATUS] ...success.")

    # use file counter for adding header row
    file_count = 0

    # open a new file to merge csv files to 
    print("[STATUS] Writing csv files to " + new_filename + "...")
    with open(new_filename, mode='w') as new_file:
        csv_writer = csv.writer(new_file, delimiter=',')
        # loop over all existing csv files listed as arguments
        for old_filename in filename_list:
            with open(old_filename) as old_file:
                csv_reader = csv.reader(old_file, delimiter=',')
                line_count = 0
                for row in csv_reader:
                    # check first row for correct headers
                    if line_count == 0:
                        assert check_csv_headers(old_filename, row) == 1
                        line_count += 1
                        # if first file add header row to new file
                        if file_count == 0:
                            csv_writer.writerow(row)
                    # add all other rows to new file
                    else:
                        csv_writer.writerow(row)
                        line_count += 1
            file_count += 1
    print("[STATUS] ...success.")

