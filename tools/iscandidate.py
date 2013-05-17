#!/usr/bin/env python2

import sys
import random

from spectra import Spectra

if len(sys.argv) != 2:
    sys.exit('Usage: %s <file>' % sys.argv[0])

spectra = Spectra().read(open(sys.argv[1], 'r'))

candidate = []
try:
    while True:
        candidate += [int(x) for x in raw_input().split()]
        try:
            while True:
                pos = candidate.index(0);
                print spectra.is_candidate(candidate[:pos])
                if pos+1 >= len(candidate):
                    candidate = []
                else:
                    candidate = candidate[pos:]
        except ValueError:
            pass
        
except EOFError:
    pass
