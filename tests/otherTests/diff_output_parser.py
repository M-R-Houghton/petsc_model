#!python3
# diff_output_parser.py

import sys

if __name__ == '__main__':

    # pipe handling - diff output should be directly piped in
    if not sys.stdin.isatty():
        diff_output = sys.stdin.read()

    # error handling - check for incorrect number of args
    elif len(sys.argv) != 2:
        sys.exit('Usage: diff_output_parser.py <diff_output>')

    # argument handling - file should be provided
    else:
        diff_output_file = sys.argv[1]

        # open provided file
        with open(diff_output_file, 'r') as fileObject:
            diff_output = fileObject.readlines()

    # diff output depends on how it was read in
    # but should be empty for success
    if diff_output == "" or diff_output == []:
        print("[PASSED]")
    else:
        print("[FAILED]")
