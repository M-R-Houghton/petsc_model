#!python3
# network_general_analysis.py

#
# Imports:
#

import sys
from network_conflict_checker import *


#
# NetworkAnalyser class:
#

# Sorting class:

class NetworkAnalyser:
    """A class for analysing the general properties of a given network

    Attributes:
        box: an instance of the Box() class.
        fibre_dict: a dictionary to be filled with fibre data.
        node_dict: a dictionary to be filled with node data.
    """

    def __init__(self, box, fibre_dict, node_dict):

        # Set a default instance of the box class:
        self.box = box

        # Set up the dictionaries:
        self.fibre_dict = fibre_dict
        self.node_dict = node_dict


    def print_basic_stats(self):

        print("Dimensions: (%f, %f, %f)" % (self.box.x, self.box.y, self.box.z))
        # number of nodes
        # total number of fibres
        # number of original fibres
        # number of cross-linking fibres

        return


    def analyse_network(self):

        # print basic network stats
        self.print_basic_stats()

        # set global min/max/avg start values
        # for each fibre

            # if normal fibre

                # set local min/max/avg start values
                # for each pair of nodes
                    # calculate the segment length
                    # update local min/max/avg

                # update global min/max/avg segment length
                # store the node IDs corresponding to min/max

            # else if cross-linking fibre
                # calculate the fibre length

        # print the segment min/max/avg
        # print the crosslink min/max/avg
        # print the overall min/max/avg



        

        return


if __name__ == '__main__':

    # Check network_general_analysis.py has a valid number of arguments:
    if len(sys.argv) != 2:
        sys.exit('Usage: network_general_analysis.py <data_file>')

    # Assign first argument:
    data_file = sys.argv[1]

    # Run the original case:
    print("READING: ", data_file)
    network_reader = NetworkReader(data_file)
    network_reader.sorter()
    print("\tREADING COMPLETE")

    print("TESTING: ", data_file)
    conflict_checker = ConflictChecker(network_reader.box, network_reader.fibre_dict, network_reader.node_dict)
    conflict_checker.check_for_conflicts()
    print("\tTEST SUCCESS")

    print("ANALYSING: ", data_file)
    network_analyser = NetworkAnalyser(network_reader.box, network_reader.fibre_dict, network_reader.node_dict)
    network_analyser.analyse_network()
    print("\tANALYSIS COMPLETE")

    # ADD FURTHER DEBUGGING PRINTS TO THIS...
