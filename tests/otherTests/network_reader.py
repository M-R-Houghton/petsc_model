#!python3
# network_reader.py

#
# Imports:
#

import sys
from common import *


#
# NetworkReader class:
#

# Sorting class:

class NetworkReader:
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
        self.link_dict = {}
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

        assert isinstance(self.box.node_count, int)
        assert isinstance(self.box.fibre_count, int)
        assert isinstance(self.box.x, float)
        assert isinstance(self.box.y, float)
        assert isinstance(self.box.z, float)
        assert isinstance(self.box.x_periodic, int)
        assert isinstance(self.box.y_periodic, int)
        assert isinstance(self.box.z_periodic, int)

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

    def add_link(self, data, l_id):
        """A function for adding an instance of the link class to the link dictionary."""

        # Any argument after the 1st is regarded as a node:
        nodes = []
        for i in range(1, len(data)):
            n_id = int(data[i])
            nodes.append(n_id)

        # Add an instance of the link class to link_dict:
        self.link_dict[l_id] = Link(*nodes)

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

        # index for links
        l_count = 0

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

            elif data[0] == 'c':

                self.add_link(data, l_count)
                l_count += 1

        assert len(data_list) == 1 + self.box.node_count + self.box.fibre_count + l_count, "Incorrect number of lines"

        return


if __name__ == '__main__':

    # Check network_reader.py has a valid number of arguments:
    if len(sys.argv) != 2:
        sys.exit('Usage: network_reader.py <data_file>')

    # Assign first argument:
    data_file = sys.argv[1]

    print("READING: ", data_file)

    # Run the original case:
    test_data = NetworkReader(data_file)
    test_data.sorter()

    # Print out all fibres
    for f1 in range(len(test_data.fibre_dict)):

        fibre1 = test_data.fibre_dict[f1]

        print(fibre1)

    # Print out all nodes
    for n1 in range(len(test_data.node_dict)):

        node1 = test_data.node_dict[n1]

        print(node1)
