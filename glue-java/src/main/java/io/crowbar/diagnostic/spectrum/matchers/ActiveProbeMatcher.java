package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * @brief This class matches all active probes in a spectrum.
 */
public final class ActiveProbeMatcher extends AbstractSpectrumMatcher<Activity, Metadata> {
    public ActiveProbeMatcher () {
        this(true);
    }

    public ActiveProbeMatcher (boolean defaultValueTransactions) {
        super(defaultValueTransactions, false);
    }

    @Override
    public BitSet matchProbes (Spectrum< ? extends Activity, ? extends Metadata> spectrum) {
        BitSet ret = new BitSet();


        for (Transaction< ? extends Activity, ? extends Metadata> t : spectrum.byTransaction()) {
            int i = 0;

            for (Activity a : t) {
                ret.set(i, ret.get(i) || a.isActive());
                i++;
            }
        }

        return ret;
    }
}