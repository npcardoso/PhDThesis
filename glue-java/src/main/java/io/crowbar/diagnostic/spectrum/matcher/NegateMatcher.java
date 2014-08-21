package io.crowbar.diagnostic.spectrum.matcher;

import io.crowbar.diagnostic.spectrum.Spectrum;
import io.crowbar.diagnostic.spectrum.Activity;
import io.crowbar.diagnostic.spectrum.Metadata;

import java.util.BitSet;

/**
 * \brief This class negates the output of some matcher.
 */
public final class NegateMatcher<A extends Activity,
                                 TM extends Metadata,
                                 CM extends Metadata>
implements SpectrumMatcher<A, TM, CM> {
    private final SpectrumMatcher< ? super A, ? super TM, ? super CM> matcher;

    public NegateMatcher (SpectrumMatcher< ? super A, ? super TM, ? super CM> matcher) {
        this.matcher = matcher;
    }

    @Override
    public BitSet matchComponents (Spectrum< ? extends A, ? extends TM, ? extends CM> spectrum) {
        BitSet ret = matcher.matchComponents(spectrum);


        ret.flip(0, spectrum.getComponentCount());

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectrum< ? extends A, ? extends TM, ? extends CM> spectrum) {
        BitSet ret = matcher.matchTransactions(spectrum);


        ret.flip(0, spectrum.getTransactionCount());
        return ret;
    }
}
