package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.matchers.AbstractSpectrumMatcher;
import io.crowbar.instrumentation.spectrum.TrM;


import java.util.BitSet;

public final class JUnitAssumeMatcher extends AbstractSpectrumMatcher<Activity, TrM> {
    public JUnitAssumeMatcher () {
        this(true);
    }

    public JUnitAssumeMatcher (boolean defaultValueProbes) {
        super(false, defaultValueProbes);
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends Activity, ? extends TrM> spectrum) {
        BitSet ret = new BitSet();

        int i = 0;


        for (Transaction< ? extends Activity, ? extends TrM> t : spectrum.byTransaction()) {
            TrM m = t.getMetadata();

            if (m != null && "org.junit.Assume$AssumptionViolatedException".equals(m.getExceptionClass()))
                ret.set(i);
            else
                ret.clear(i);

            i++;
        }

        return ret;
    }
}