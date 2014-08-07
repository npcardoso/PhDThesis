package io.crowbar.instrumentation.spectra;

import io.crowbar.diagnosis.spectra.HitActivity;
import io.crowbar.diagnosis.spectra.HitSpectraView;
import io.crowbar.diagnosis.spectra.matcher.SpectraMatcher;

import java.util.SortedSet;

public class SpectraView
extends HitSpectraView<TransactionMetadata, ComponentMetadata>
implements Spectra {
    public SpectraView (Spectra spectra,
                        SpectraMatcher< ? super HitActivity, ? super TransactionMetadata, ? super ComponentMetadata> matcher) {
        super(spectra, matcher);
    }
}