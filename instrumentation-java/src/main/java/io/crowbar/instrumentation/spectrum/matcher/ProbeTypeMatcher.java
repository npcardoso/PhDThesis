package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.matchers.AbstractSpectrumMatcher;
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
            ProbeType type = spectrum.getProbe(i).getType();

            if (types.contains(type))
                ret.set(i);
        }

        return ret;
    }
}