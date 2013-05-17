class Spectra:
    
    def __init__(self, ncomps=0, ntrans=0, activity=None, error=None):
        self.ncomps = ncomps
        self.ntrans = ntrans

        if activity is None:
            self.activity = ntrans * ncomps * [0];
        else:
            self.activity = activity

        if error is None:
            self.error = ntrans * [0]
        else:
            self.error = error

    def set(self, comp, trans, val=1):
        self.activity[(trans - 1) * self.ncomps + (comp - 1)] = val

    def set_error(self, trans, val=1):
        self.error[(trans - 1)] = val
    
    def get(self, comp, trans):
        return self.activity[(trans - 1) * self.ncomps + (comp - 1)]

    def get_error(self, trans):
        return self.error[(trans - 1)]

    def write(self):
        print self.ncomps, self.ntrans
        for transaction in range(1, self.ntrans + 1):
            for component in range(1, self.ncomps + 1):
                print self.get(component, transaction),
            print int(self.get_error(transaction))

    def read(self, file):
        buffer = str()
        for line in file:
            buffer += " "+line

        buffer = buffer.split();
        
        self.ncomps = int(buffer[0])
        self.ntrans = int(buffer[1])
        
        self.activity = self.ntrans * self.ncomps * [0]
        self.error = self.ntrans * [0]
        
        i = 2
        for transaction in range(1, self.ntrans + 1):
            for component in range(1, self.ncomps + 1):
                self.set(component, transaction, int(buffer[i]))
                i = i + 1
            self.set_error(transaction, buffer[i] == '1' or buffer[i] == 'x' or buffer[i] == '-');
            i = i + 1

        return self

    def is_candidate(self, candidate):
        for transaction in range(1, self.ntrans + 1):
            if not self.get_error(transaction) > 0:
                continue
            hit = False
            for component in candidate:
                if(self.get(component, transaction) > 0):
                    hit = True
                    break
            if not hit:
                return False
        return True

