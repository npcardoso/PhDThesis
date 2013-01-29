import sys
import random

if len(sys.argv) != 2:
    sys.exit('Usage: %s <file>' % sys.argv[0])


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

def read_spectra(file):
    buffer = str()
    for line in file:
        buffer += " "+line

    buffer = [int(x) for x in buffer.split()];

    component_count = int(buffer[0])
    transaction_count = int(buffer[1])

    activity = buffer[2:2 + transaction_count * component_count]
    error = buffer[2 + transaction_count * component_count:]
    return Spectra(component_count, transaction_count, activity, error)

def is_candidate(spectra, candidate):
    for transaction in range(1, spectra.ntrans + 1):
        if not spectra.get_error(transaction) > 0:
            continue
        hit = False
        for component in candidate:
            if(spectra.get(component, transaction) > 0):
                hit = True
                break
        if not hit:
            return False
    return True
    

spectra = read_spectra(open(sys.argv[1], 'r'))

try:
    while True:
        candidate = [int(x) for x in input().split()]
        print(is_candidate(spectra, candidate))
        
except EOFError:
    pass
