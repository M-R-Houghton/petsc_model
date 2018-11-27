#!python3
# readerwriter.py

#
# Imports:
#

import sys


#
# Model class:
#

class ReadAndWrite:
    """A class for reading in a data file and writing it back out.

    Attributes:
        data_file: a data file containing network data.
    """

    def __init__(self, data_file):

        self.data_file = data_file

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

        # Open the new file:
        data_file = data_file[:-1] + 'e'
        f = open(data_file, 'w')

        # strings to store recorded
        failures = ""
        skipped = ""

        for line in data_list:

            if ("[  FAILED  ]" in line) and (line not in failures):
                if "listed below" in line:
                    f.write(line[:-7] + ' above')
                else:
                    f.write(line)
                f.write('\n')
                failures += line

            if ("[  SKIPPED ]" in line) and (line not in skipped):
                if "listed below" in line:
                    f.write(line[:-7] + ' above')
                else:
                    f.write(line)
                f.write('\n')
                skipped += line

        f.close()

        return

#
# Command line support:
#

if __name__ == '__main__':

    # Check model.py has a valid number of arguments:
    if len(sys.argv) != 2:
        sys.exit('USAGE: readerwriter.py <data_file>')

    # Assign first argument:
    data = sys.argv[1]

    # Create instance of the DataWriter class:
    mn = ReadAndWrite(data)

