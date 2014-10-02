package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * @brief This class matches all active probes in a spectrum.
 */
public final class ActiveProbeMatcher extends AbstractSpectrumMatcher {
    public ActiveProbeMatcher () {
        this(true);
    }

    public ActiveProbeMatcher (boolean defaultValueTransactions) {
        super(defaultValueTransactions, false);
    }

    @Override
    public BitSet matchProbes (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (Transaction t : spectrum.byTransaction()) {
            for (Integer id : t.getActivity()) {
                ret.set(id);
            }
        }

        return ret;
    }
}