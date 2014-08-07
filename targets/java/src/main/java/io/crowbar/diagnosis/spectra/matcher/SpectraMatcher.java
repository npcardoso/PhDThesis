package io.crowbar.diagnosis.spectra.matcher;

import io.crowbar.diagnosis.spectra.Spectra;
import io.crowbar.diagnosis.spectra.Activity;
import io.crowbar.diagnosis.spectra.Metadata;

public interface SpectraMatcher<A extends Activity,
                                TM extends Metadata,
                                CM extends Metadata> {
    boolean matchComponent (Spectra< ? extends A, ? extends TM, ? extends CM> spectra,
                            int componentId);
    boolean matchTransaction (Spectra< ? extends A, ? extends TM, ? extends CM> spectra,
                              int transactionId);
}