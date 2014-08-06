package io.crowbar.instrumentation.spectra;

import io.crowbar.diagnosis.spectra.HitSpectraView;

import java.util.SortedSet;

public class SpectraView
extends HitSpectraView<TransactionMetadata, ComponentMetadata>
implements Spectra {
    public SpectraView (Spectra spectra,
                        SortedSet<Integer> components,
                        SortedSet<Integer> transactions) {
        super(spectra, components, transactions);
    }
}