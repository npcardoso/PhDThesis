package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Transaction;
import java.util.BitSet;

/**
 * \brief This class matches transactions that have at least one component activation.
 */
public final class ValidTransactionMatcher
extends AbstractSpectraMatcher<Activity, Metadata, Metadata> {
    public ValidTransactionMatcher () {
        this(true);
    }

    public ValidTransactionMatcher (boolean defaultValueComponents) {
        super(false, defaultValueComponents);
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends Activity, ? extends Metadata, ? extends Metadata> spectra) {
        BitSet ret = new BitSet();

        int i = 0;


        for (Transaction< ? extends Activity, ? extends Metadata> t : spectra.byTransaction()) {
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
