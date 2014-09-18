package io.crowbar.instrumentation.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Transaction;
import io.crowbar.diagnostic.spectrum.matchers.AbstractSpectrumMatcher;
import io.crowbar.instrumentation.spectrum.TrM;


import java.util.BitSet;

public final class JUnitAssumeMatcher extends AbstractSpectrumMatcher {
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


        // TODO: Fix this
        /*
         *      for (Transaction t : spectrum.byTransaction()) {
         *          TrM m = t.getMetadata();
         *          if (m != null && "org.junit.Assume$AssumptionViolatedException".equals(m.getExceptionClass()))
         *              ret.set(i);
         *          else
         *              ret.clear(i);
         *
         *          i++;
         *      }
         */
        return ret;
    }
}