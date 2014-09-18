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
    public TestProbesMatcher () {
        super(true, false);
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet testNodes = new BitSet();


        // Mark test nodes
        for (Probe p : spectrum.byProbe()) {
            if (p.getType() == ProbeType.TRANSACTION_START)
                testNodes.set(p.getNode().getId());
        }

        // Mark test nodes' children
        for (Node n : spectrum.getTree()) {
            Node parent = n.getParent();

            if (parent != null && testNodes.get(parent.getId()))
                testNodes.set(n.getId());
        }


        // create probe matcher
        BitSet ret = new BitSet();

        for (Probe p : spectrum.byProbe()) {
            if (testNodes.get(p.getNode().getId()))
                ret.set(p.getId());
        }

        return ret;
    }
}