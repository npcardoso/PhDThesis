package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.matcher.AbstractSpectrumMatcher;
import io.crowbar.instrumentation.spectrum.TrM;


import java.util.BitSet;

public final class JUnitAssumeMatcher extends AbstractSpectrumMatcher<Activity, TrM, Metadata> {
    public JUnitAssumeMatcher () {
        this(true);
    }

    public JUnitAssumeMatcher (boolean defaultValueComponents) {
        super(false, defaultValueComponents);
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends Activity, ? extends TrM, ? extends Metadata> spectrum) {
        BitSet ret = new BitSet();

        int i = 0;


        for (Transaction< ? extends Activity, ? extends TrM> t : spectrum.byTransaction()) {
            if ("org.junit.Assume$AssumptionViolatedException".equals(t.getMetadata().getExceptionClass()))
                ret.set(i);
            else
                ret.clear(i);

            i++;
        }

        return ret;
    }
}
