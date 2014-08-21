package io.crowbar.diagnostic.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches all active components in a spectrum.
 */
public final class ActiveComponentMatcher extends AbstractSpectrumMatcher<Activity, Metadata, Metadata> {
    public ActiveComponentMatcher () {
        this(true);
    }

    public ActiveComponentMatcher (boolean defaultValueTransactions) {
        super(defaultValueTransactions, false);
    }

    @Override
    public BitSet matchComponents (Spectrum< ? extends Activity, ? extends Metadata, ? extends Metadata> spectrum) {
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
