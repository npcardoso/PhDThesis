package io.crowbar.diagnosis.spectra;

import java.util.SortedSet;
public class HitSpectraView<TM extends Metadata,
                            CM extends Metadata>
extends SpectraView<HitActivity, TM, CM>
implements HitSpectra<TM, CM> {
    public HitSpectraView (HitSpectra<TM, CM> spectra,
                           SortedSet<Integer> components,
                           SortedSet<Integer> transactions) {
        super(spectra, components, transactions);
    }
}