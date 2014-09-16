package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches probes that were active in at least one failing transaction.
 */
public final class SuspiciousProbeMatcher
extends AbstractSpectrumMatcher<Activity, Metadata> {
    public SuspiciousProbeMatcher () {
        this(true);
    }

    public SuspiciousProbeMatcher (boolean defaultValueTransactions) {
        super(defaultValueTransactions, false);
    }

    @Override
    public BitSet matchProbes (Spectrum< ? extends Activity, ? extends Metadata> spectrum) {
        BitSet ret = new BitSet();


        for (Transaction< ? extends Activity, ? extends Metadata> t : spectrum.byTransaction()) {
            if (!t.isError())
                continue;

            int i = 0;

            for (Activity a : t) {
                if (a.isActive())
                    ret.set(i);

                i++;
            }
        }

        return ret;
    }
}