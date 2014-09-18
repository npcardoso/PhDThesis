package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.matchers.AbstractSpectrumMatcher;
import io.crowbar.instrumentation.spectrum.HitTransactionWithException;


import java.util.BitSet;

public final class JUnitAssumeMatcher extends AbstractSpectrumMatcher {
    public static final String ASSUME_CLASS = "org.junit.Assume$AssumptionViolatedException";
    public JUnitAssumeMatcher () {
        this(true);
    }

    public JUnitAssumeMatcher (boolean defaultValueProbes) {
        super(false, defaultValueProbes);
    }

    @Override
    public BitSet matchTransactions (Spectrum spectrum) {
        BitSet ret = new BitSet();

        int i = 0;


        for (Transaction t : spectrum.byTransaction()) {
            if (t instanceof HitTransactionWithException) {
                String exClass = ((HitTransactionWithException) t).getExceptionClass();

                if (ASSUME_CLASS.equals(exClass))
                    ret.set(t.getId());
            }
        }

        return ret;
    }
}