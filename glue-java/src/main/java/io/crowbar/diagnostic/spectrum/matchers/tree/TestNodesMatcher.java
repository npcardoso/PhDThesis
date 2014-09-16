package io.crowbar.diagnostic.spectrum.matchers.tree;

import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Spectrum;

import java.util.TreeSet;

public class TestNodesMatcher extends AbstractNodeMatcher {
    TreeSet<Integer> permanentIds = new TreeSet<Integer> ();

    public TestNodesMatcher (Spectrum< ? extends Activity, ? extends Metadata> spectrum) {
        for (Probe p : spectrum.byProbe()) {
            if (p.getType() == ProbeType.TRANSACTION_START)
                permanentIds.add(p.getNode().getPermanentId());
        }
    }

    @Override
    protected boolean keepIfNoChildren () {
        return false;
    }

    @Override
    protected boolean matchNode (Node n) {
        return !permanentIds.contains(n.getPermanentId());
    }
}