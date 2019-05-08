#!/usr/bin/env python3
# node_displacement_checker.py

# Imports
import sys
from network_reader import *

class DisplacementChecker:

    def __init__(self, filename):

        self.inp_file = filename + "_in.dat"
        self.out_file = filename + "_out.dat"

    def check_displacements(self):

        print("Checking displacements...")
        inp_net = self.create_network(self.inp_file)
        out_net = self.create_network(self.out_file)

        return

    def create_network(self, net_file):

        print("Creating networks...")

        net_dat = NetworkReader(net_file)
        net_dat.sorter()

        return net_dat



if __name__ == '__main__':

    # Check node_displacement_checker.py has a valid number of arguments
    if len(sys.argv) != 2:
        sys.exit('Usage: node_displacement_checker.py <arg1>')

    arg1 = sys.argv[1]

    checker = DisplacementChecker(arg1)
    checker.check_displacements()

