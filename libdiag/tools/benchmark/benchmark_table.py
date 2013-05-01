#!/usr/bin/env python2

import sys
import random


class Item:
    def __init__(self, ncomp, ntran, time):
        self.ncomp = ncomp
        self.ntran = ntran
        self.time = time

    def __repr__(self):
        return "%s, %s, %f" % (self.ncomp, self.ntran, self.time)


def read_spectra(file):
    buffer = str()
    results = []

    for line in file:
        line = line.split(";")
        id = line[0].split("x")
        times = [float(x) for x in line[1:]]
        results.append(Item(id[0], id[1], sum(times) / len(times)))

    return results


results = read_spectra(sys.stdin)
print('"Components", "Transactions", "Time"')
for i in results:
    print(i)

