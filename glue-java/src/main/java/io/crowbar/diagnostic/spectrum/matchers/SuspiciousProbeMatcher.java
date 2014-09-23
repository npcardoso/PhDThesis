package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches probes that were active in at least one failing transaction.
 */
public final class SuspiciousProbeMatcher
extends AbstractSpectrumMatcher {
    public SuspiciousProbeMatcher () {
        this(true);
    }

    public SuspiciousProbeMatcher (boolean defaultValueTransactions) {
        super(defaultValueTransactions, false);
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (Transaction t : spectrum.byTransaction()) {
            if (!t.isError())
                continue;

            for (Integer i : t.getActivity()) {
                ret.set(i);
            }
        }

        return ret;
    }
}