package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;
import java.util.BitSet;

public abstract class AbstractSpectraMatcher<A extends Activity,
                                             TM extends Metadata,
                                             CM extends Metadata>
implements SpectraMatcher<A, TM, CM> {
    private final boolean defaultValueComponents;
    private final boolean defaultValueTransactions;

    protected AbstractSpectraMatcher () {
        this(false, false);
    }

    protected AbstractSpectraMatcher (boolean defaultValueTransactions,
                                      boolean defaultValueComponents) {
        this.defaultValueTransactions = defaultValueTransactions;
        this.defaultValueComponents = defaultValueComponents;
    }

    @Override
    public BitSet matchComponents (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = new BitSet();


        if (defaultValueComponents) {
            ret.set(0, spectra.getNumComponents());
        }

        return ret;
    }

    @Override
    public BitSet matchTransactions (Spectra< ? extends A, ? extends TM, ? extends CM> spectra) {
        BitSet ret = new BitSet();


        if (defaultValueTransactions) {
            ret.set(0, spectra.getNumTransactions());
        }

        return ret;
    }
}