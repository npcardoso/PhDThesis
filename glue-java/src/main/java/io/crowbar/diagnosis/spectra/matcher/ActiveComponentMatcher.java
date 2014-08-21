package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches all active components in a spectra.
 */
public final class ActiveComponentMatcher extends AbstractSpectraMatcher<Activity, Metadata, Metadata> {
    public ActiveComponentMatcher () {
        this(true);
    }

    public ActiveComponentMatcher (boolean defaultValueTransactions) {
        super(defaultValueTransactions, false);
    }

    @Override
    public BitSet matchComponents (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra) {
        BitSet ret = new BitSet();


        for (Transaction< ? extends Activity, ? extends Metadata> t : spectra.byTransaction()) {
            int i = 0;

            for (Activity a : t) {
                ret.set(i, ret.get(i) || a.isActive());
                i++;
            }
        }

        return ret;
    }
}
