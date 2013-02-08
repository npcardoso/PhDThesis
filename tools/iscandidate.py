#!/usr/bin/env python2

import sys
import random

from spectra import Spectra

if len(sys.argv) != 2:
    sys.exit('Usage: %s <file>' % sys.argv[0])

spectra = Spectra().read(open(sys.argv[1], 'r'))

try:
    while True:
        candidate = [int(x) for x in raw_input().split()]
        print spectra.is_candidate(candidate)
        
except EOFError:
    pass
