#!/usr/bin/env python2

import sys
import random

from spectra import Spectra
                
buffer = str()
activity = []
error = []

for line in file:
    line = line.split()
    activity += [int(x) for x in line[0:-1]];
    error += [line[-1] == '-']

Spectra(len(activity) / len(error) , len(error), activity, error).write()

