#!python3
# network_general_analysis.py

#
# Imports:
#

import sys
from network_conflict_checker import *


#
# NetworkAnalyser class:
#

# Sorting class:

class NetworkAnalyser:
    """A class for analysing the general properties of a given network

    Attributes:
        box: an instance of the Box() class.
        fibre_dict: a dictionary to be filled with fibre data.
        node_dict: a dictionary to be filled with node data.
    """

    def __init__(self, box, fibre_dict, node_dict):

        # Set a default instance of the box class:
        self.box = box

        # Set up the dictionaries:
        self.fibre_dict = fibre_dict
        self.node_dict = node_dict


    def find_mag(self, vec):
        return np.sqrt(vec.dot(vec))


    def find_dist_vec(self, s_alph, s_beta):
        """Returns the distance vector from alpha to beta, accounting for periodic boundaries."""

        # Declaring the direction of the fibre:
        s_alph_beta = s_beta - s_alph

        # Check if fibre extends over the boundary:
        for i in range(len(self.box.dimensions)):

            if s_alph_beta[i] > 0.5 * self.box.dimensions[i]:
                s_alph_beta[i] -= self.box.dimensions[i]
            elif s_alph_beta[i] < -0.5 * self.box.dimensions[i]:
                s_alph_beta[i] += self.box.dimensions[i]

        return s_alph_beta


    def print_basic_stats(self):

        print("Dimensions: (%f, %f, %f)" % (self.box.x, self.box.y, self.box.z))
        # number of nodes
        # total number of fibres
        # number of original fibres
        # number of cross-linking fibres

        return

    def update_min(self, minimum, value):

        if value < minimum:     # should also return IDs
            minimum = value

        return minimum

    def update_max(self, maximum, value):

        if value > maximum:     # should also return IDs
            maximum = value

        return maximum


    def analyse_crosslinks(self):

        # set sum of no. of crosslinks = 0
        # set global min/max/avg start values
        # for each fibre
        global_seg_min, global_seg_max, global_seg_avg = self.box.x+self.box.y, 0, 0

        total_normal_fibres = 0

        for f_i in range(len(self.fibre_dict)):

            fibre_i = self.fibre_dict[f_i]

            # analysis for normal fibres
            if len(fibre_i.nodes) > 2:

                total_normal_fibres += 1

                local_seg_min, local_seg_max, local_seg_avg = self.box.x+self.box.y, 0, 0

                for n in range(len(fibre_i.nodes) - 1):

                    alph = self.node_dict[fibre_i.nodes[n]]
                    beta = self.node_dict[fibre_i.nodes[n+1]]

                    s_alph_beta = self.find_dist_vec(alph.s_xyz, beta.s_xyz)

                    mag_s_alph_beta = self.find_mag(s_alph_beta)

                    local_seg_min = self.update_min(local_seg_min, mag_s_alph_beta)
                    local_seg_max = self.update_max(local_seg_max, mag_s_alph_beta)

                    local_seg_avg += mag_s_alph_beta

                local_seg_avg /= float(len(fibre_i.nodes) - 1)

                #print("local min = %f" % local_seg_min)
                #print("local max = %f" % local_seg_max)
                #print("local avg = %f" % local_seg_avg)

                global_seg_min = self.update_min(global_seg_min, local_seg_min)
                global_seg_max = self.update_max(global_seg_max, local_seg_max)

                global_seg_avg += local_seg_avg

                # update global min/max/avg segment length
                # store the node IDs corresponding to min/max

            # else if cross-linking fibre
                # calculate the fibre length
                
        global_seg_avg /= float(total_normal_fibres)

        print("global min = %f" % global_seg_min)
        print("global max = %f" % global_seg_max)
        print("global avg = %f" % global_seg_avg)

        # set avg no. of cross-links per fibre = sum / no. of fibres

        # print the segment min/max/avg
        # print the crosslink min/max/avg
        # print the overall min/max/avg

        return


    def analyse_network(self):

        # print basic network stats
        self.print_basic_stats()

        # perform initial crosslink analysis
        self.analyse_crosslinks()

        # calculate theoretical coordination number

        # calculate actual coordination number

        # calculate fibre density

        # calculate the affine energy/shear modulus for given gamma
        # (to be investigated/implemented at a later point)
        

        return


if __name__ == '__main__':

    # Check network_general_analysis.py has a valid number of arguments:
    if len(sys.argv) != 2:
        sys.exit('Usage: network_general_analysis.py <data_file>')

    # Assign first argument:
    data_file = sys.argv[1]

    # Run the original case:
    print("READING: ", data_file)
    network_reader = NetworkReader(data_file)
    network_reader.sorter()
    print("\tREADING COMPLETE")

    print("TESTING: ", data_file)
    conflict_checker = ConflictChecker(network_reader.box, network_reader.fibre_dict, network_reader.node_dict)
    conflict_checker.check_for_conflicts()
    print("\tTEST SUCCESS")

    print("ANALYSING: ", data_file)
    network_analyser = NetworkAnalyser(network_reader.box, network_reader.fibre_dict, network_reader.node_dict)
    network_analyser.analyse_network()
    print("\tANALYSIS COMPLETE")

    # ADD FURTHER DEBUGGING PRINTS TO THIS...
