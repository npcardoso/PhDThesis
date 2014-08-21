package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;

import java.util.BitSet;

/**
 * \brief This class negates the output of some matcher.
 */
public final class NegateMatcher<A extends Activity,
                                 TM extends Metadata,
                                 CM extends Metadata>
implements SpectraMatcher<A, TM, CM> {
    private final SpectraMatcher< ? super A, ? super TM, ? super CM> matcher;

    public NegateMatcher (SpectraMatcher< ? super A, ? super TM, ? super CM> matcher) {
        this.matcher = matcher;
    }

    @Override
    public BitSet matchComponents (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = matcher.matchComponents(spectra);


        ret.flip(0, spectra.getComponentCount());

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = matcher.matchTransactions(spectra);


        ret.flip(0, spectra.getTransactionCount());
        return ret;
    }
}
