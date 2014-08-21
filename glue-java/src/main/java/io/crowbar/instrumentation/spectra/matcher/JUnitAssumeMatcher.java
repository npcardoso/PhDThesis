package io.crowbar.instrumentation.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import io.crowbar.diagnosis.spectra.Transaction;
import io.crowbar.diagnosis.spectra.matcher.AbstractSpectraMatcher;
import io.crowbar.instrumentation.spectra.TrM;


import java.util.BitSet;

public final class JUnitAssumeMatcher extends AbstractSpectraMatcher<Activity, TrM, Metadata> {
    public JUnitAssumeMatcher () {
        this(true);
    }

    public JUnitAssumeMatcher (boolean defaultValueComponents) {
        super(false, defaultValueComponents);
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends Activity, ? extends TrM, ? extends Metadata> spectra) {
        BitSet ret = new BitSet();

        int i = 0;


        for (Transaction< ? extends Activity, ? extends TrM> t : spectra.byTransaction()) {
            if ("org.junit.Assume$AssumptionViolatedException".equals(t.getMetadata().getExceptionClass()))
                ret.set(i);
            else
                ret.clear(i);

            i++;
        }

        return ret;
    }
}
