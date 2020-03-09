#!python3

import numpy as np

class Node:

    def __init__(self, nodeID, nodeType, x, y, z):
        self.__id = nodeID
        self.__xyz = np.array((x, y, z))
    
    def setPosition(self, x, y, z):
        self.__xyz = np.array((x, y, z))

    def setX(self, x):
        self.__xyz[0] = x

    def setY(self, y):
        self.__xyz[1] = y

    def setZ(self, z):
        self.__xyz[2] = z

    def getPosition(self):
        return(self.__xyz)

        

