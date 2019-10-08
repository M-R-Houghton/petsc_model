#!/usr/bin/env python3
# generate_par_file.py

# Imports
import sys
import re
import os

class ParGenerator:

    def __init__(self, filename_in_dat):

        self.filename_in_dat = filename_in_dat
        self.filename_base = self.get_base_name()
        self.filename_par = self.get_par_name() 
        self.filename_out_dat = re.sub('_in','_out',self.filename_in_dat)
        self.filename_res = self.get_res_name()

    def get_base_name(self):
        strip_path = os.path.basename(self.filename_in_dat)
        strip_base = re.sub('_in\.dat$','',strip_path)
        return strip_base

    def get_par_name(self):
        sub_name = os.path.split(os.path.dirname(self.filename_in_dat))[-1]
        par_name = sub_name + '/' + self.filename_base + '.par'
        return par_name

    def get_res_name(self):
        res_name = re.sub('dat/', 'res/', self.filename_in_dat)
        res_name = re.sub('_in\.dat$', '.csv', res_name)
        return res_name

    def write_par_file(self):

        with open(self.filename_par, 'w') as f_object:
            f_object.write(self.filename_in_dat+'\n')
            f_object.write(self.filename_out_dat+'\n')
            f_object.write(self.filename_res+'\n')

        return


if __name__ == '__main__':

    # Check generate_par_file.py has a valid number of arguments
    if len(sys.argv) != 2:
        sys.exit('Usage: generate_par_file.py <filename_in_dat>')

    filename_in_dat = sys.argv[1]
    par_generator = ParGenerator(filename_in_dat)
    par_generator.write_par_file()

