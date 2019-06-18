#!/usr/bin/env python3
# network_id_shifter.py

# Imports
import sys
from network_reader import *



if __name__ == '__main__':

    # Check network_id_shifter.py has a valid number of arguments
    if len(sys.argv) != 2:
        sys.exit('Usage: network_id_shifter.py <arg1>')

    # Assign first argument:
    data_file = sys.argv[1]

    # Assign node and fibre ID shifts
    node_shift = 1686
    fibre_shift = 200
    network_data = []

    # Modify the input file and save to a list of split strings
    with open(data_file, 'r') as fObj:
        for line in fObj:
            split_line = line.split()
            if line[0] == 'f':
                split_line[1] = str(int(split_line[1])+fibre_shift)
                for i in range(3, len(split_line)):
                    split_line[i] = str(int(split_line[i])+node_shift)
            if line[0] == 'n':
                split_line[1] = str(int(split_line[1])+node_shift)
            network_data.append(split_line)

    # Write out the list of split strings as a temporary output file
    with open('tmp.dat', 'w') as fObj:
        for line in network_data:
            for bit in line:
                fObj.write(bit + ' ')
            fObj.write('\n')
