package io.crowbar.diagnosis.spectra;

import io.crowbar.diagnosis.spectra.matcher.SpectraMatcher;

import java.util.SortedSet;

public class HitSpectraView<TM extends Metadata,
                            CM extends Metadata>
extends SpectraView<HitActivity, TM, CM>
implements HitSpectra<TM, CM> {
    public HitSpectraView (HitSpectra<TM, CM> spectra,
                           SpectraMatcher< ? super HitActivity, ? super TM, ? super CM> matcher) {
        super(spectra, matcher);
    }
}