#!python3
# network_conflict_checker.py

#
# Imports:
#

import sys
from network_reader import *


#
# ConflictChecker class:
#

# Sorting class:

class ConflictChecker:
    """A class for checking the validity of a given network

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


    def check_for_box_conflicts(self):

        # Test for consistent number of fibres and nodes
        assert len(self.fibre_dict) == self.box.fibre_count, '(ERROR) Conflicting number of fibres'
        assert len(self.node_dict) == self.box.node_count, '(ERROR) Conflicting number of nodes'

        return


    def check_for_fibre_conflicts(self):

        # Test for consistent fibres
        for f1 in range(len(self.fibre_dict)):

            fibre1 = self.fibre_dict[f1]

            for f2 in range(len(self.fibre_dict)):

                if f1 != f2:

                    fibre2 = self.fibre_dict[f2]

                    assert fibre1.radius == fibre2.radius, '(ERROR) Conflicting radius between two fibres (ID=%r and ID=%r).' % (fibre1.id, fibre2.id)

                    assert fibre1.id != fibre2.id, '(ERROR) Two fibres with the same ID (ID=%r).' % fibre1.id

                    if len(fibre1.nodes) == len(fibre2.nodes):

                        same_node_list = True
                        
                        for n in range(len(fibre1.nodes)):

                            if fibre1.nodes[n] != fibre2.nodes[n]:
                                same_node_list = False

                            # test for internally located boundary and dangling nodes
                            if n > 0 and n < len(fibre1.nodes) - 1:
                                nID = fibre1.nodes[n]
                                assert self.node_dict[nID].type != 2, '(ERROR) Invalid boundary node (ID=%r) found.' % nID
                                assert self.node_dict[nID].type != 1, '(ERROR) Invalid dangling node (ID=%r) found.' % nID


                        assert same_node_list == False, '(ERROR) Two fibres with same node list.'

        return


    def check_for_node_conflicts(self):

        # Test for consistent x and y coordinates
        for n1 in range(len(self.node_dict)):

            node1 = self.node_dict[n1]

            for n2 in range(len(self.node_dict)):

                if n1 != n2:

                    node2 = self.node_dict[n2]

                    assert node1.id != node2.id, '(ERROR) Two nodes with identical IDs (ID=%r).' % node1.id
                    #print("x1 = ", node1.x, "x2 = ", node2.x)
                    #print("y1 = ", node1.y, "y2 = ", node2.y)
                    #print("z1 = ", node1.z, "z2 = ", node2.z)
                    assert (node1.x != node2.x) or (node1.y != node2.y) or (node1.z != node2.z), '(ERROR) Two identical nodes (ID=%r and ID=%r).' % (node1.id, node2.id)

        return


    def check_for_conflicts(self):
        """Performs assertions on box, fibres and nodes."""

        self.check_for_box_conflicts()
        self.check_for_fibre_conflicts()
        self.check_for_node_conflicts()

        return


if __name__ == '__main__':

    # Check network_conflict_checker.py has a valid number of arguments:
    if len(sys.argv) != 2:
        sys.exit('Usage: network_conflict_checker.py <data_file>')

    # Assign first argument:
    data_file = sys.argv[1]

    # Run the original case:
    read_data = NetworkReader(data_file)
    read_data.sorter()

    print("TESTING: ", data_file)
    test_data = ConflictChecker(read_data.box, read_data.fibre_dict, read_data.node_dict)
    test_data.check_for_conflicts()
    print("\tTEST SUCCESS")

    # ADD FURTHER DEBUGGING PRINTS TO THIS...
