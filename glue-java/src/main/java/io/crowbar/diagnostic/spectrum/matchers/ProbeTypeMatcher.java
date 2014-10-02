package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Probe;
import io.crowbar.diagnostic.spectrum.ProbeType;


import java.util.BitSet;
import java.util.Set;
import java.util.HashSet;

public final class ProbeTypeMatcher extends AbstractSpectrumMatcher {
    private final Set<ProbeType> types = new HashSet<ProbeType> ();
    public ProbeTypeMatcher (ProbeType... types) {
        this(true, types);
    }

    public ProbeTypeMatcher (boolean defaultValueTransactions,
                             ProbeType... types) {
        super(defaultValueTransactions, false);

        for (ProbeType t : types) {
            this.types.add(t);
        }
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (int i = 0; i < spectrum.getProbeCount(); i++) {
            Probe p = spectrum.getProbe(i);

            if (p == null)
                continue;

            ProbeType type = p.getType();

            if (types.contains(type))
                ret.set(i);
        }

        return ret;
    }
}