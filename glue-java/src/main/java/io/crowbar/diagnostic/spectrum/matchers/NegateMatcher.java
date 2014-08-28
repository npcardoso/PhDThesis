package io.crowbar.diagnostic.spectrum.matchers;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;

import java.util.BitSet;

/**
 * \brief This class negates the output of some matcher.
 */
public final class NegateMatcher<A extends Activity,
                                 TM extends Metadata>
implements SpectrumMatcher<A, TM> {
    private final SpectrumMatcher< ? super A, ? super TM> matcher;

    public NegateMatcher (SpectrumMatcher< ? super A, ? super TM> matcher) {
        this.matcher = matcher;
    }

    @Override
    public BitSet matchProbes (Spectrum< ? extends A, ? extends TM> spectrum) {
        BitSet ret = matcher.matchProbes(spectrum);


        ret.flip(0, spectrum.getProbeCount());

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends A, ? extends TM> spectrum) {
        BitSet ret = matcher.matchTransactions(spectrum);


        ret.flip(0, spectrum.getTransactionCount());

        return ret;
    }
}