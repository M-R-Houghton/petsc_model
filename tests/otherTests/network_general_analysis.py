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

        # set a default instance of the box class:
        self.box = box

        # set up the dictionaries:
        self.fibre_dict = fibre_dict
        self.node_dict = node_dict

        self.global_seg_min = 0
        self.global_seg_max = 0
        self.global_seg_avg = 0

        self.global_fib_min = 0
        self.global_fib_max = 0
        self.global_fib_avg = 0

        self.num_crosslink_min = self.box.node_count
        self.num_crosslink_max = 0
        self.num_crosslink_avg = 0

        self.boundary_fibres = []

        self.further_analysis = False

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


    def update_min(self, minimum, value):

        if value < minimum:     # should also return IDs
            minimum = value

        return minimum


    def update_max(self, maximum, value):

        if value > maximum:     # should also return IDs
            maximum = value

        return maximum


    def update_fib_count(self, total_normal_fibres, total_crosslink_fibres):
        self.box.total_normal_fibres = total_normal_fibres
        self.box.total_crosslink_fibres = total_crosslink_fibres
        return 


    def update_global_seg(self, global_seg_min, global_seg_max, global_seg_avg):
        self.global_seg_min = global_seg_min
        self.global_seg_max = global_seg_max
        self.global_seg_avg = global_seg_avg
        return 

    def update_global_fib(self, global_fib_min, global_fib_max, global_fib_avg):
        self.global_fib_min = global_fib_min
        self.global_fib_max = global_fib_max
        self.global_fib_avg = global_fib_avg
        return 


    def analyse_nodes(self):

        for n in range(len(self.node_dict)):

            node = self.node_dict[n]

            if node.type == 0:
                self.box.total_internal_node_count += 1

            if node.type == 1:
                self.box.total_dangling_node_count += 1

            if node.type == 2:
                self.box.total_boundary_node_count += 1

        return


    def analyse_segment_lengths(self, fibre, seg_min, seg_max, seg_avg):

        for n in range(len(fibre.nodes) - 1):

            alph = self.node_dict[fibre.nodes[n]]
            beta = self.node_dict[fibre.nodes[n+1]]

            s_alph_beta = self.find_dist_vec(alph.s_xyz, beta.s_xyz)

            mag_s_alph_beta = self.find_mag(s_alph_beta)

            seg_min = self.update_min(seg_min, mag_s_alph_beta)
            seg_max = self.update_max(seg_max, mag_s_alph_beta)

            seg_avg += mag_s_alph_beta

        seg_avg /= float(len(fibre.nodes) - 1)

        return {'min':seg_min, 'max':seg_max, 'avg':seg_avg}


    def analyse_number_of_crosslinks(self, fibre):

        num_crosslink = 0
        for i in fibre.nodes:

            if self.node_dict[i].type != 1:
                num_crosslink += 1

        self.num_crosslink_min = self.update_min(self.num_crosslink_min, num_crosslink)
        self.num_crosslink_max = self.update_max(self.num_crosslink_max, num_crosslink)
        self.num_crosslink_avg += num_crosslink

        return


    def analyse_crosslinks(self):

        # set sum of no. of crosslinks = 0
        # set global min/max/avg start values
        # for each fibre
        global_seg_min, global_seg_max, global_seg_avg = self.box.x+self.box.y, 0, 0

        global_fib_min, global_fib_max, global_fib_avg = self.box.x+self.box.y, 0, 0

        total_normal_fibres, total_crosslink_fibres = 0, 0

        for f_i in range(len(self.fibre_dict)):

            fibre_i = self.fibre_dict[f_i]

            # analysis for normal fibres
            if len(fibre_i.nodes) > 2:

                total_normal_fibres += 1

                self.analyse_number_of_crosslinks(fibre_i)

                local_seg = self.analyse_segment_lengths(fibre_i, self.box.x+self.box.y, 0, 0)

                #print("local min = %f" % local_seg['min'])
                #print("local max = %f" % local_seg['max'])
                #print("local avg = %f" % local_seg['avg'])

                global_seg_min = self.update_min(global_seg_min, local_seg['min'])
                global_seg_max = self.update_max(global_seg_max, local_seg['max'])

                global_seg_avg += local_seg['avg']

                # store the node IDs corresponding to min/max

            # analysis for crosslinking fibres
            else:

                assert len(fibre_i.nodes) == 2, '(ERROR) Fibre with less than 2 nodes.'

                total_crosslink_fibres += 1

                # calculate the fibre length
                alph = self.node_dict[fibre_i.nodes[0]]
                beta = self.node_dict[fibre_i.nodes[1]]

                s_alph_beta = self.find_dist_vec(alph.s_xyz, beta.s_xyz)

                mag_s_alph_beta = self.find_mag(s_alph_beta)

                global_fib_min = self.update_min(global_fib_min, mag_s_alph_beta)
                global_fib_max = self.update_max(global_fib_max, mag_s_alph_beta)

                global_fib_avg += mag_s_alph_beta

                # store ID of boundary fibres
                if alph.type == 2 and beta.type == 2:
                    self.boundary_fibres.append(f_i)

        self.num_crosslink_avg /= total_normal_fibres

        assert total_normal_fibres + total_crosslink_fibres == self.box.fibre_count, '(ERROR) Fibre not counted.'

        if total_normal_fibres != 0:
            global_seg_avg /= float(total_normal_fibres)
            self.update_global_seg(global_seg_min, global_seg_max, global_seg_avg)

        if total_crosslink_fibres != 0:
            global_fib_avg /= float(total_crosslink_fibres)
            self.update_global_fib(global_fib_min, global_fib_max, global_fib_avg)

        self.update_fib_count(total_normal_fibres, total_crosslink_fibres)

        return


    def analyse_network(self):

        # perform basic analysis
        self.analyse_nodes()

        # perform initial crosslink analysis
        self.analyse_crosslinks()

        # print basic network stats
        self.print_basic_stats()

        # print crosslink analysis
        self.print_crosslink_stats()

        # calculate theoretical coordination number

        # calculate actual coordination number

        # calculate fibre density

        # calculate the affine energy/shear modulus for given gamma
        # (to be investigated/implemented at a later point)
        

        return


    def analyse_node(self, nodeID):

        fID = []
        nid_all_lists = []
        seg_all_lists = []
        seg_avg_lists = []

        # for the fibres that contain the node of interest
        for f_i in range(len(self.fibre_dict)):

            fibre_i = self.fibre_dict[f_i]

            for n_j in fibre_i.nodes:

                if n_j == nodeID:
                    fID.append(f_i)
                    segs = self.analyse_fibre(n_j, f_i)
                    nid_all_lists.append(segs['nid'])
                    seg_all_lists.append(segs['all'])
                    seg_avg_lists.append(segs['avg'])

        return {'fid':fID, 'nid':nid_all_lists, 'seg_all':seg_all_lists, 'seg_avg':seg_avg_lists}


    def analyse_fibre(self, nodeID, fibreID):

        fibre = self.fibre_dict[fibreID]

        ids_all = []
        seg_all = []
        seg_avg = 0

        for n in range(len(fibre.nodes) - 1):

            alph = self.node_dict[fibre.nodes[n]]
            beta = self.node_dict[fibre.nodes[n+1]]

            s_alph_beta = self.find_dist_vec(alph.s_xyz, beta.s_xyz)

            mag_s_alph_beta = self.find_mag(s_alph_beta)

            ids_all.append(fibre.nodes[n])
            seg_all.append(mag_s_alph_beta)
            seg_avg += mag_s_alph_beta

        ids_all.append(fibre.nodes[-1])

        seg_avg /= float(len(fibre.nodes) - 1)
        
        return {'nid':ids_all, 'all':seg_all, 'avg':seg_avg}


    def print_basic_stats(self):

        print("Dimensions:\t(%f, %f, %f)" % (self.box.x, self.box.y, self.box.z))
        print("Periodic:\t(%s, %s, %s)" % (bool(self.box.x_periodic), bool(self.box.y_periodic), bool(self.box.z_periodic)))

        print("Total fibre count = %d" % self.box.fibre_count)
        print("Original fibre count = %d" % self.box.total_normal_fibres)
        print("Crosslink fibre count = %d" % self.box.total_crosslink_fibres)

        print("Total node count = %d" % self.box.node_count)
        print("Internal node count = %d" % self.box.total_internal_node_count)
        print("Boundary node count = %d" % self.box.total_boundary_node_count)
        print("Dangling node count = %d" % self.box.total_dangling_node_count)
        # number of nodes
        # total number of fibres
        # number of original fibres
        # number of cross-linking fibres

        return


    def print_crosslink_stats(self):

        print("Segment length min = %f" % self.global_seg_min)
        print("Segment length max = %f" % self.global_seg_max)
        print("Segment length avg = %f" % self.global_seg_avg)

        print("Crosslink length min = %f" % self.global_fib_min)
        print("Crosslink length max = %f" % self.global_fib_max)
        print("Crosslink length avg = %f" % self.global_fib_avg)

        print("Number of crosslinks per fibre min = %d" % self.num_crosslink_min)
        print("Number of crosslinks per fibre max = %d" % self.num_crosslink_max)
        print("Number of crosslinks per fibre avg = %f" % self.num_crosslink_avg)

        print("Total boundary crosslinks = %d" % len(self.boundary_fibres))

        return


    def write_basic_stats(self, file):

        file.write("Dimensions:\t(%f, %f, %f)\n" % (self.box.x, self.box.y, self.box.z))
        file.write("Periodic:\t(%s, %s, %s)\n" % (bool(self.box.x_periodic), bool(self.box.y_periodic), bool(self.box.z_periodic)))

        file.write("Total fibre count = %d\n" % self.box.fibre_count)
        file.write("Original fibre count = %d\n" % self.box.total_normal_fibres)
        file.write("Crosslink fibre count = %d\n" % self.box.total_crosslink_fibres)

        file.write("Total node count = %d\n" % self.box.node_count)
        file.write("Internal node count = %d\n" % self.box.total_internal_node_count)
        file.write("Boundary node count = %d\n" % self.box.total_boundary_node_count)
        file.write("Dangling node count = %d\n" % self.box.total_dangling_node_count)

        return


    def write_crosslink_stats(self, file):

        file.write("Segment length min = %f\n" % self.global_seg_min)
        file.write("Segment length max = %f\n" % self.global_seg_max)
        file.write("Segment length avg = %f\n" % self.global_seg_avg)

        file.write("Crosslink length min = %f\n" % self.global_fib_min)
        file.write("Crosslink length max = %f\n" % self.global_fib_max)
        file.write("Crosslink length avg = %f\n" % self.global_fib_avg)

        file.write("Number of crosslinks per fibre min = %d\n" % self.num_crosslink_min)
        file.write("Number of crosslinks per fibre max = %d\n" % self.num_crosslink_max)
        file.write("Number of crosslinks per fibre avg = %f\n" % self.num_crosslink_avg)

        file.write("Total boundary crosslinks = %d\n" % len(self.boundary_fibres))

        return




    def write_node_stats(self, nodeID, file):
        
        file.write("Node ID = %d\n" % nodeID)
        
        segs = self.analyse_node(nodeID)

        for f in range(len(segs['fid'])):
            
            file.write("Fibre ID = %d\n" % segs['fid'][f])
            file.write("Node IDs are:\n")
            file.write("\t"+str(segs['nid'][f])+"\n")
            file.write("Segment lengths are:\n")
            file.write("\t"+str(segs['seg_all'][f])+"\n")
            file.write("Segment Avg = %.4f\n" % segs['seg_avg'][f])
            #l = [float('%.6g' % 1.0/i) for i in segs['seg_all'][f]]
            l = [1.0/i for i in segs['seg_all'][f]]
            l_avg = sum(l)/len(l)
            file.write("1/l is:\n")
            file.write("\t"+str(l)+"\n")
            file.write("1/l avg is: %.4f\n" % l_avg)
            #l3 = [float('%.6g' % 1.0/(i**3)) for i in segs['seg_all'][f]]
            l3 = [1.0/(i**3) for i in segs['seg_all'][f]]
            l3_avg = sum(l3)/len(l3)
            file.write("1/l**3 is:\n")
            file.write("\t"+str(l3)+"\n")
            file.write("1/l**3 avg is: %.4f\n" % l3_avg)
        return


    def write_network_stats(self, output_file):

        new_file = open(output_file, 'w')

        # write basic network stats
        self.write_basic_stats(new_file)

        # write crosslink analysis
        self.write_crosslink_stats(new_file)

        if self.further_analysis:
            # write out additional data
            for i in self.nodes_to_analyse:
                self.write_node_stats(i, new_file)

        new_file.close()

        return


if __name__ == '__main__':

    # Check network_general_analysis.py has a valid number of arguments:
    if (len(sys.argv) < 2) or (len(sys.argv) > 4):
        sys.exit('Usage: network_general_analysis.py <input_file> [output_file]')

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

    assert len(conflict_checker.boundary_fibres) == len(network_analyser.boundary_fibres), '(ERROR) Boundary fibre detected.'

    if len(sys.argv) == 4:
        id_list = map(int, sys.argv[3].strip('[]').split(','))
        print("FURTHER ANALYSIS: ")
        network_analyser.further_analysis = True
        network_analyser.nodes_to_analyse = id_list
        print("\tFURTHER ANALYSIS")

    if len(sys.argv) > 2:
        output_file = sys.argv[2]
        print("WRITING LOG FILE: ", output_file)
        network_analyser.write_network_stats(output_file)
        print("\tWRITING COMPLETE")

    # ADD FURTHER DEBUGGING PRINTS TO THIS...







