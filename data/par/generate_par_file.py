#!python3
# generate_par_file.py

# Imports
import sys
import re
import os

class ParGenerator:

    def __init__(self, filename_in_dat):

        self.filename_in_dat = filename_in_dat
        self.filename_base = self.get_basename()
        self.filename_par = self.filename_base + '.par'
        self.filename_out_dat = re.sub('_in','_out',self.filename_in_dat)

    def get_basename(self):
        strip_path = os.path.basename(self.filename_in_dat)
        strip_base = re.sub('_in\.dat$','',strip_path)
        return strip_base

    def write_par_file(self):

        with open(self.filename_par, 'w') as f_object:
            f_object.write(self.filename_in_dat+'\n')
            f_object.write(self.filename_out_dat+'\n')

        return


if __name__ == '__main__':

    # Check generate_par_file.py has a valid number of arguments
    if len(sys.argv) != 2:
        sys.exit('Usage: generate_par_file.py <filename_in_dat>')

    filename_in_dat = sys.argv[1]
    par_generator = ParGenerator(filename_in_dat)
    par_generator.write_par_file()

