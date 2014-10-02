package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;
import java.util.BitSet;

public final  class SpectrumViewFactory {
    private final Spectrum spectrum;

    private final BitSet transactions = new BitSet();
    private final BitSet probes = new BitSet();

    public SpectrumViewFactory (Spectrum spectrum) {
        this.spectrum = spectrum;
        transactions.set(0, spectrum.getTransactionCount());
        probes.set(0, spectrum.getProbeCount());
    }

    public SpectrumView getView () {
        return new SpectrumView(this.spectrum,
                                this.transactions,
                                this.probes);
    }

    public void addStage (SpectrumMatcher matcher) {
        SpectrumView view = getView();


        BitSet retTr = matcher.matchTransactions(view);


        assert (retTr != null);
        assert (retTr.length() <= view.getTransactionCount());

        for (int i = 0; i < view.getTransactionCount(); i++) {
            if (!retTr.get(i)) {
                int original = view.getTransactionMapping(i);
                transactions.clear(original);
            }
        }

        BitSet retPr = matcher.matchProbes(view);
        assert (retPr != null);
        assert retPr.length() <= view.getProbeCount();

        for (int i = 0; i < view.getProbeCount(); i++) {
            if (!retPr.get(i)) {
                int original = view.getProbeMapping(i);
                probes.clear(original);
            }
        }
    }
}