#!/usr/bin/python3
import numpy as np
import socket, math

class Bone:
    def __init__ (self, x1, y1, z1, x2, y2, z2):
        self.location = np.array([(x1, y1, z1), (x2, y2, z2)])
        self.line = np.zeros( (1, 3) )

    def getDirection (self):
        self.line = self.location[1] - self.location[0]

class Hand:
    def __init__ (self):
        self.start = np.zeros( (1, 3) )
        self.location = self.start

    def calculateMove (self, foreArm, upperArm):
        pass
        # TODO: calculate the new directions and movements of the bones

def main():
    hand = Hand()
    forearm = Bone(0, -3, 0, 0, -33, 0)
    upperarm = Bone(0, -33, 0, 0, -63, 0)
    

    while True:
        pass


main()
