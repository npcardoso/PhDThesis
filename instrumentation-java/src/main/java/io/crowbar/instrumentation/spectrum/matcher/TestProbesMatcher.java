package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.matchers.AbstractSpectrumMatcher;

import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;

public class TestProbesMatcher extends AbstractSpectrumMatcher {
    Map<Integer, Boolean> map;

    public TestProbesMatcher () {
        super(true, false);
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        map = new HashMap<Integer, Boolean> ();

        for (int i = 0; i < spectrum.getProbeCount(); i++) {
            if (!check(spectrum, spectrum.getProbe(i).getNode()))
                ret.set(i);
        }

        return ret;
    }

    public boolean check (Spectrum spectrum,
                          Node n) {
        if (n == null) return false;

        if (map.containsKey(n.getId())) return map.get(n.getId());

        boolean result = false;

        for (Probe p : spectrum.byProbe()) {
            if (p.getNodeId() == n.getId() && p.getType() == ProbeType.TRANSACTION_START) {
                result = true;
                break;
            }
        }

        if (!result) {
            result = check(spectrum, n.getParent());
        }

        map.put(n.getId(), result);

        return result;
    }
}