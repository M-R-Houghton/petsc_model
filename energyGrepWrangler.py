#!/usr/bin/env python3
# energyGrepWrangler.py

# Imports
import sys

def rel_error(actual, estimate):
    return abs(actual - estimate) / actual
    
def rel_error_em(estimate):
    actual = 1.1107210122196359e-10
    return rel_error(actual, estimate)

def rel_error_ts(estimate):
    actual = 1.1107210122196359e-10
    return rel_error(actual, estimate)


if __name__ == '__main__':

    data = sys.stdin.read()
    print(data)

    data = data.split('= ')
    print(data)
    data = data[-1].split('\n')
    estimate = float(data[0])

    print("{:e}".format(estimate))
    #print("{:e}".format(rel_error_em(estimate)))

