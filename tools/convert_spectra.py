#!/usr/bin/env python2

import sys
import random


class Spectra:
    def __init__(self, ncomps, ntrans, activity, error):
        self.ncomps = ncomps
        self.ntrans = ntrans
        self.activity = activity
        self.error = error

    def get(self, comp, trans):
        return self.activity[(trans - 1) * self.ncomps + (comp - 1)]

    def get_error(self, trans):
        return self.error[(trans - 1)]

    def write(self):
        print self.ncomps, self. ntrans
        for transaction in range(1, self.ntrans + 1):
            for component in range(1, self.ncomps + 1):
                print self.get(component, transaction),
            print
        
        for transaction in range(1, self.ntrans + 1):
            print int(self.get_error(transaction)),
        print
                
def read_spectra(file):
    buffer = str()
    activity = []
    error = []
    
    for line in file:
        line = line.split()
        activity += [int(x) for x in line[0:-1]];
        error += [line[-1] == '-']

    return Spectra(len(activity) / len(error) , len(error), activity, error)


read_spectra(sys.stdin).write()

