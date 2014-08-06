package io.crowbar.diagnosis.spectra;

import java.util.SortedSet;

public class HitSpectraView<M> extends SpectraView<HitActivity, M> implements HitSpectra<M> {
    public HitSpectraView (HitSpectra<M> spectra,
                           SortedSet<Integer> components,
                           SortedSet<Integer> transactions) {
        super(spectra, components, transactions);
    }
}