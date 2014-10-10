package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches transactions that have at least one probe activation.
 */
public final class ValidTransactionMatcher
extends AbstractSpectrumMatcher {
    public ValidTransactionMatcher () {
        this(true);
    }

    public ValidTransactionMatcher (boolean defaultValueProbes) {
        super(false, defaultValueProbes);
    }

    @Override
    public BitSet matchTransactions (Spectrum spectrum) {
        BitSet ret = new BitSet();


        for (Transaction t : spectrum.byTransaction()) {
            boolean valid = t.getActivity().iterator().hasNext();
            ret.set(t.getId(), valid);
        }

        return ret;
    }
}