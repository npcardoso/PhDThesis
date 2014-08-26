package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;
import java.util.BitSet;

public final  class SpectrumViewFactory<A extends Activity,
                                        TM extends Metadata> {
    private final Spectrum<A, TM> spectrum;

    private final BitSet transactions = new BitSet();
    private final BitSet probes = new BitSet();

    public SpectrumViewFactory (Spectrum<A, TM> spectrum) {
        this.spectrum = spectrum;
        transactions.set(0, spectrum.getTransactionCount());
        probes.set(0, spectrum.getProbeCount());
    }

    public SpectrumView<A, TM> getView () {
        return new SpectrumView<A, TM> (this.spectrum,
                                        this.transactions,
                                        this.probes);
    }

    public void addStage (SpectrumMatcher< ? super A, ? super TM> matcher) {
        SpectrumView<A, TM> view = getView();


        BitSet retTr = matcher.matchTransactions(view);
        assert (retTr.length() < view.getTransactionCount());

        for (int i = 0; i < view.getTransactionCount(); i++) {
            if (!retTr.get(i)) {
                int original = view.getTransactionMapping(i);
                transactions.clear(original);
            }
        }

        BitSet retComp = matcher.matchProbes(view);
        assert (retComp.length() < view.getProbeCount());

        for (int i = 0; i < view.getProbeCount(); i++) {
            if (!retComp.get(i)) {
                int original = view.getProbeMapping(i);
                probes.clear(original);
            }
        }
    }
}