#!python3
# testForConflicts.py

#
# Imports:
#

import sys
from common import *


#
# ConflictChecker class:
#

# Sorting class:

class ConflictChecker:
    """A class for reading in the data files

    Attributes:
        box: an instance of the Box() class. Set to zeros until overwritten.
        data_file: a data file containing unsorted fibre/node/connection data.
        fibre_dict: a dictionary to be filled with fibre data.
        node_dict: a dictionary to be filled with node data.
        connection_dict: a dictionary to be filled with connection data.
        boundary: an instance of the Boundary() class.
    """

    def __init__(self, data_file):

        # Store the data file:
        self.data_file = data_file

        self.n_types = [0, 1, 2]

        # Set a default instance of the box class:
        self.box = Box(0, 0, 0, 0, 0, 0, 0, 0)            # Program will fail if box line is not first

        # Initialise the dictionaries:
        self.fibre_dict = {}
        self.node_dict = {}

    def create_box(self, data):
        """A function for creating an instance of the box class from some given data."""

        # Type casting:
        n_count = int(data[1])
        f_count = int(data[2])
        x_dim = float(data[3])
        y_dim = float(data[4])
        z_dim = float(data[5])
        x_per = int(data[6])
        y_per = int(data[7])
        z_per = int(data[8])

        self.box = Box(n_count, f_count, x_dim, y_dim, z_dim, x_per, y_per, z_per)

        return

    def add_fibre(self, data):
        """A function for adding an instance of the fibre class to the fibre dictionary."""

        # Type casting:
        f_id = int(data[1])
        radius = float(data[2])

        # Any argument after the 2nd is regarded as a fibre:
        nodes = []
        for i in range(3, len(data)):
            n_id = int(data[i])
            nodes.append(n_id)

        # Add an instance of the fibre class to fibre_dict:
        self.fibre_dict[f_id] = Fibre(f_id, radius, *nodes)

        return

    def add_node(self, data):
        """A function for adding an instance of the node class to the fibre dictionary."""

        # Type casting:
        n_id = int(data[1])
        n_type = int(data[2])
        x = float(data[3])
        y = float(data[4])
        z = float(data[5])

        # Temporarily save the current node:
        current_node = Node(n_id, n_type, x, y, z)

        # Add an instance of the node class to node_dict:
        self.node_dict[n_id] = current_node

        return

    def sorter(self):
        """Reads the data file and sorts the data into the relevant dictionaries."""

        # Print progress message:
        #print('Attempting to retrieve data...')

        # Attempt to read in data file:
        try:
            f = open(self.data_file)
            data_list = f.read().splitlines()
            f.close()

        except FileNotFoundError:
            error_message = 'File ' + str(self.data_file) + ' was not found.'
            sys.exit(error_message)

        # Print progress message:
        #print('Sorting data into dictionaries...')

        # Loop through the data:
        for data in data_list:
            data = data.split()

            # Determine box dimensions:
            if data[0] == 'b':

                self.create_box(data)

            # Add fibres to fibre dictionary:
            elif data[0] == 'f':

                self.add_fibre(data)

            # Add nodes to node dictionary:
            elif data[0] == 'n':

                self.add_node(data)

        return


if __name__ == '__main__':

    # Check testForConflicts.py has a valid number of arguments:
    if len(sys.argv) != 2:
        sys.exit('Usage: testForConflicts.py <data_file>')

    # Assign first argument:
    data_file = sys.argv[1]

    # Run the original case:
    test_data = ConflictChecker(data_file)
    test_data.sorter()

    print("TESTING: ", data_file)

    # Test for consistent number of fibres and nodes
    assert len(test_data.fibre_dict) == test_data.box.fibre_count, '(ERROR) Conflicting number of fibres'
    assert len(test_data.node_dict) == test_data.box.node_count, '(ERROR) Conflicting number of nodes'

    # Test for consistent fibres
    for f1 in range(len(test_data.fibre_dict)):

        fibre1 = test_data.fibre_dict[f1]

        #if len(fibre1.nodes) == 3:
            #print("Found a fibre of length 3:")
            #print(fibre1.id)

        for f2 in range(len(test_data.fibre_dict)):

            if f1 != f2:

                fibre2 = test_data.fibre_dict[f2]

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
                            assert test_data.node_dict[nID].type != 2, '(ERROR) Invalid boundary node (ID=%r) found.' % nID
                            assert test_data.node_dict[nID].type != 1, '(ERROR) Invalid dangling node (ID=%r) found.' % nID


                    assert same_node_list == False, '(ERROR) Two fibres with same node list.'


    # Test for consistent x and y coordinates
    for n1 in range(len(test_data.node_dict)):

        node1 = test_data.node_dict[n1]

        for n2 in range(len(test_data.node_dict)):

            if n1 != n2:

                node2 = test_data.node_dict[n2]

                assert node1.id != node2.id, '(ERROR) Two nodes with identical IDs (ID=%r).' % node1.id
                #print("x1 = ", node1.x, "x2 = ", node2.x)
                #print("y1 = ", node1.y, "y2 = ", node2.y)
                #print("z1 = ", node1.z, "z2 = ", node2.z)
                assert (node1.x != node2.x) or (node1.y != node2.y) or (node1.z != node2.z), '(ERROR) Two identical nodes (ID=%r and ID=%r).' % (node1.id, node2.id)

    print("\tTEST SUCCESS")

    # ADD FURTHER DEBUGGING PRINTS TO THIS...
