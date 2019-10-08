#!/usr/bin/env python3
# merge_csv_files.py

# Imports
import sys
import csv

if __name__ == '__main__':

    # Check merge_csv_files.py has a valid number of arguments
    if len(sys.argv) < 2:
        sys.exit('Usage: merge_csv_files.py <new_file.csv> <file1.csv> <file2.csv>')

    new_file = sys.argv[1]
    file1 = sys.argv[2]

    file_list = []
    file_list.append(file1)

    with open(new_file, mode='w') as out_file:
        csv_writer = csv.writer(out_file, delimiter=',')
        file_count = 0
        for old_file in file_list:
            with open(old_file) as csv_file:
                csv_reader = csv.reader(csv_file, delimiter=',')
                line_count = 0
                for row in csv_reader:
                    if line_count == 0:
                        print('Column names are {}'.format(", ".join(row)))
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
                        assert row[12] == "EnergyAffn"
                        assert row[13] == "ShearModulus"
                        assert row[14] == "ShearModAffn"
                        if file_count == 0:
                            csv_writer.writerow(row)
                        line_count += 1
                    else:
                        csv_writer.writerow(row)
                        line_count += 1
                print('Processed {} lines.'.format(line_count))
            file_count += 1

