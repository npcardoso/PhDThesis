package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches transactions that have at least one component activation.
 */
public final class ValidTransactionMatcher
extends AbstractSpectrumMatcher<Activity, Metadata> {
    public ValidTransactionMatcher () {
        this(true);
    }

    public ValidTransactionMatcher (boolean defaultValueComponents) {
        super(false, defaultValueComponents);
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends Activity, ? extends Metadata> spectrum) {
        BitSet ret = new BitSet();

        int i = 0;


        for (Transaction< ? extends Activity, ? extends Metadata> t : spectrum.byTransaction()) {
            boolean valid = false;

            for (Activity a : t) {
                if (a.isActive()) {
                    valid = true;
                    break;
                }
            }

            ret.set(i++, valid);
        }

        return ret;
    }
}
