#!python3
# rad_writer.py

#
# Imports
#

import sys


#
# Model class
#

class WriteRadius:
    """A class for reading in a data file and writing it back out with a new radius.

    Attributes:
        data_file: a data file containing network data.
        radius: the desired radius.
    """

    def __init__(self, data_file, radius):

        self.data_file = data_file
        self.radius = radius

        # Read in data
        data_list = self.read_file(self.data_file)

        # Write out data
        self.write_file(self.data_file, data_list)


    def read_file(self, data_file):

    	# Attempt to read in data file
        try:
            f = open(self.data_file)
            data_list = f.read().splitlines()
            f.close()

        except FileNotFoundError:
            sys.exit('File was not found.')

        return data_list


    def write_file(self, data_file, data_list):

        # Open the new file
        f = open(data_file, 'w')

        # Loop through by line
        for line in data_list:

            # Update radius on each fibre line
            if line[0] == 'f':
                data = line.split()

                for i in range(len(data)):

                    if i == 2:
                        f.write(str(self.radius) + ' ')
                    else:
                        f.write(data[i] + ' ')

                f.write('\n')

            # Don't change any of the other lines
            else:
                f.write(line)
                f.write('\n')

        # Close the file
        f.close()

        return

#
# Command line support
#

if __name__ == '__main__':

    # Check model.py has a valid number of arguments
    if (len(sys.argv) != 3):
        sys.exit('USAGE: rad_writer.py <data_file> <radius>')

    # Assign first argument
    data = sys.argv[1]
    rad = sys.argv[2]

    # Create instance of the DataWriter class
    mn = WriteRadius(data, rad)

