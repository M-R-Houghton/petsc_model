#!python3
# diff_output_parser.py

import sys

if __name__ == '__main__':

    # Check network_general_analysis.py has a valid number of arguments:
    if not sys.stdin.isatty():
        diff_output = sys.stdin.read()
    elif len(sys.argv) != 2:
        sys.exit('Usage: diff_output_parser.py <diff_output>')
    else:
        diff_output_file = sys.argv[1]

        with open(diff_output_file, 'r') as fileObject:
            diff_output = fileObject.readlines()

    if diff_output == []:
        print("[PASSED]")
    else:
        print("[FAILED]")
