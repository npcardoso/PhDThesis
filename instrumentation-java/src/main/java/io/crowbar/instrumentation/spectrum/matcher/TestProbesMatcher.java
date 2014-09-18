package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.matchers.AbstractSpectrumMatcher;

import java.util.BitSet;

public class TestProbesMatcher extends AbstractSpectrumMatcher {
    public TestProbesMatcher () {
        super(true, false);
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (int i = 0; i < spectrum.getProbeCount(); i++) {
            if (check(spectrum, spectrum.getProbe(i).getNode()))
                ret.clear(i);
            else
                ret.set(i);
        }

        return ret;
    }

    public boolean check (Spectrum spectrum,
                          Node n) {
        if (n == null) return false;

        for (Probe p : spectrum.getNodeProbes(n.getId())) {
            if (p.getType() == ProbeType.TRANSACTION_START) return true;
        }

        return check(spectrum, n.getParent());
    }
}