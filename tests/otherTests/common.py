#!python3
# common.py
# Format v2

#
# Imports:
#
import numpy as np

# REQUIRED MODULES #


#
# Box class:
#

class Box:
    """A class for the box containing the fibres and nodes

    Attributes:
        node_count: the number of nodes in the network
        fibre_count: the number of fibres in the network
        x: the x dimension of the box
        y: the y dimension of the box
        z: the z dimension of the box
        x_periodic: whether x is periodic or not
        y_periodic: whether y is periodic or not
        z_periodic: whether z is periodic or not
    """

    def __init__(self, nc, fc, x, y, z, x_periodic, y_periodic, z_periodic):
        self.node_count = nc
        self.fibre_count = fc
        self.x = x
        self.y = y
        self.z = z
        self.x_periodic = x_periodic
        self.y_periodic = y_periodic
        self.z_periodic = z_periodic

        # added to original to make array manipulation easier
        self.dimensions = np.array((x,y,z))
        self.periodicity = np.array((x_periodic,y_periodic,z_periodic))

    def __str__(self):
        statement = 'Total nodes: \t' + str(self.node_count) + '\n' \
                    'Total fibres: \t' + str(self.fibre_count) + '\n' \
                    'Box dimensions: \t(' + str(self.x) + ' x ' \
                      + str(self.y) + ' x ' + str(self.z) + ')\n\n' \
                    'Periodicity: \n' \
                    '\tx periodic: \t' + str(bool(self.x_periodic)) + '\n' \
                    '\ty periodic: \t' + str(bool(self.y_periodic)) + '\n' \
                    '\tz periodic: \t' + str(bool(self.z_periodic)) + '\n'

        return statement


#
# Fibre class:
#

class Fibre:
    """A new class for fibres

    Attributes:
        id: an integer identifying the fibre
        radius: the radius of the fibre
        *nargs: an ordered list of node IDs associated with the fibre
    """

    def __init__(self, fibre_id, radius, *nargs):
        self.id = fibre_id
        self.radius = radius
        self.nodes = []

        for node in nargs:
            self.nodes.append(node)

    def __str__(self):
        statement = 'Fibre ID: ' + str(self.id) + '\n' \
                    + '\t radius: \t\t' + str(self.radius) + '\n' \
                    + '\t node IDs: \t' \
                    + str(self.nodes) + '\n'

        return statement


#
# Node class:
#

class Node:
    """A class for nodes

    Attributes:
        id: an integer identifying the node
        x: the x coordinate of the node
        y: the y coordinate of the node
        z: the z coordinate of the node
    """

    def __init__(self, node_id, node_type, x, y, z):
        self.id = node_id
        self.type = node_type
        self.x = x
        self.y = y
        self.z = z

        # added to original to make array manipulation easier
        self.s_xyz = np.array((x,y,z))
        self.u_xyz = np.array((0,0,0))

    def __str__(self):
        statement = 'Node ID: ' + str(self.id) + '\n' \
                    + '\t node type: \t' + str(self.type) \
                    + '\t coordinates: \t('+str(self.x)+', '+str(self.y)+', '+str(self.z)+')\n'

        return statement


#
# Node displacement class:
#

class NodeDisplacement:
    """A class for node displacement

    Attributes:
        id: an integer identifying the node
        x: the x displacement of the node
        y: the y displacement of the node
        z: the z displacement of the node
    """

    def __init__(self, node_id, x, y, z):
        self.id = node_id
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        statement = 'Node ID: ' + str(self.id) + '\n' \
                    + '\t displacements: \t('+str(self.x)+', '+str(self.y)+', '+str(self.z)+')\n'

        return statement


#
# Segment classes:
#

class SegPair:
    """A class for node pair segments

    Attributes:
        id: an integer identifying the node
        radius: the radius of the segment
        node_a: the node ID of the first end node
        node_b: the node ID of the second end node
        l_ab: the segment length
        n_ab: the vector norm of the segment
    """

    def __init__(self, seg_id, radius, node_a, node_b, seg_length, n_ab):
        self.id = seg_id
        self.radius = radius
        self.node_a = node_a
        self.node_b = node_b
        self.l_ab = seg_length
        self.n_ab = n_ab


class SegTriplet:
    """A class for two adjacent node pair segments on the same fibre,
        i.e. a node triplet

    Attributes:
        id: an integer identifying the node
        radius: the radius of the segment
        node_a: the node ID of the first end node
        node_b: the node ID of the middle node
        node_c: the node ID of the second end node
        l_abc: the average length of the two segments
        n_ac: the vector norm across both segments
    """

    def __init__(self, seg_id, radius, node_a, node_b, node_c, av_seg_length, n_ac):
        self.id = seg_id
        self.radius = radius
        self.node_a = node_a
        self.node_b = node_b
        self.node_c = node_c
        self.l_abc = av_seg_length
        self.n_ac = n_ac
