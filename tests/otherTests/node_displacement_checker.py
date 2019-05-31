#!/usr/bin/env python3
# node_displacement_checker.py

# Imports
import sys
from network_reader import *

class DisplacementChecker:

    def __init__(self, filename, tolerance=0.1):

        self.inp_file = filename + "_in.dat"
        self.out_file = filename + "_out.dat"

        self.disp_tol = float(tolerance)

    def check_displacements(self):

        print("Checking displacements...")
        inp_net = self.create_network(self.inp_file)
        out_net = self.create_network(self.out_file)

        self.diff_displacements(inp_net, out_net)

        return

    def diff_displacements(self, inp_net, out_net):

        print("Differencing node coordinates...")

        large_disp_count = 0
        max_ind = -1
        max_diff = [0,0,0]

        assert len(inp_net.node_dict) == len(out_net.node_dict), "ERROR: inconsistent node list lengths" 
        for index in range(len(inp_net.node_dict)):
            inp_node = inp_net.node_dict[index]
            out_node = out_net.node_dict[index]

            dx = self.diff_value(inp_node.x, out_node.x)
            dy = self.diff_value(inp_node.y, out_node.y)
            dz = self.diff_value(inp_node.z, out_node.z)

            tol = self.disp_tol
            if dx > tol*inp_net.box.x or dy > tol*inp_net.box.y or dz > tol*inp_net.box.z:

                max_ind, max_diff = self.update_max(max_ind, max_diff, index, [dx,dy,dz])
                large_disp_count += 1
                
                # Print all large diffs
                #self.print_node(inp_node, out_node, [dx,dy,dz])

        self.print_summary(inp_net, out_net, large_disp_count, max_ind, max_diff)

        return

    def update_max(self, max_index, max_diff, cur_index, cur_diff):

        assert len(max_diff) == 3
        assert len(cur_diff) == 3

        for i in range(len(max_diff)):
            if max_diff[i] < cur_diff[i]:
                max_diff[i] = cur_diff[i]
                max_index = cur_index

        return max_index, max_diff

    def print_summary(self, inp_net, out_net, large_disp_count, max_ind, max_diff):

        print("Found "+str(large_disp_count)+" large displacements.")
        print("Out of "+str(inp_net.box.node_count)+" nodes.")
        print("With a tolerance of "+str(int(self.disp_tol*100))+"%.")

        if max_ind == -1:
            print("All nodes are within the tolerance.")
        else:
            print("Worst node is:")
            self.print_node(inp_net.node_dict[max_ind], out_net.node_dict[max_ind], max_diff)

        return

    def print_node(self, inp_node, out_node, disp):
        assert len(disp) == 3, "ERROR: displacement vec has wrong dimensions"

        dx,dy,dz = disp[0],disp[1],disp[2]

        print('node ID: '+str(inp_node.id)+'\n' \
                +'\tnode type:    \t '+str(inp_node.type)+'\n' \
                +'\tinp coords:   \t(' \
                +str('%.6f'%inp_node.x)+', '+str('%.6f'%inp_node.y)+', '+str('%.6f'%inp_node.z)+')\n' \
                +'\tout coords:   \t(' \
                +str('%.6f'%out_node.x)+', '+str('%.6f'%out_node.y)+', '+str('%.6f'%out_node.z)+')\n' \
                +'\tdisplacement: \t(' \
                +str('%.6f'%dx)+', '+str('%.6f'%dy)+', '+str('%.6f'%dz)+')\n')

        return

    def diff_value(self, inp_val, out_val):

        return abs(inp_val - out_val)

    def create_network(self, net_file):

        print("Creating networks...")

        net_dat = NetworkReader(net_file)
        net_dat.sorter()

        return net_dat



if __name__ == '__main__':

    # dummy default tol
    arg2 = 0.1

    # Check node_displacement_checker.py has a valid number of arguments
    if len(sys.argv) == 3:
        arg2 = sys.argv[2]
    elif len(sys.argv) != 2:
        sys.exit('Usage: node_displacement_checker.py <filename> [tol]')

    arg1 = sys.argv[1]

    checker = DisplacementChecker(arg1, arg2)
    checker.check_displacements()

