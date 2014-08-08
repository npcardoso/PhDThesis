package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;

import java.util.BitSet;

public interface SpectraMatcher<A extends Activity,
                                TM extends Metadata,
                                CM extends Metadata> {
    BitSet matchComponents (Spectra< ? extends A, ? extends TM, ? extends CM> spectra);
    BitSet matchTransactions (Spectra< ? extends A, ? extends TM, ? extends CM> spectra);
}