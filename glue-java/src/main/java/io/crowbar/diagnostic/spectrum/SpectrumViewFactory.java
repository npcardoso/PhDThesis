package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.spectrum.matchers.SpectrumMatcher;
import java.util.BitSet;

public final  class SpectrumViewFactory<A extends Activity,
                                       TM extends Metadata> {
    private final Spectrum<A, TM> spectrum;

    private final BitSet transactions = new BitSet();
    private final BitSet components = new BitSet();

    public SpectrumViewFactory (Spectrum<A, TM> spectrum) {
        this.spectrum = spectrum;
        transactions.set(0, spectrum.getTransactionCount());
        components.set(0, spectrum.getComponentCount());
    }

    public SpectrumView<A, TM> getView () {
        return new SpectrumView<A, TM> (this.spectrum,
                                           this.transactions,
                                           this.components);
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

        BitSet retComp = matcher.matchComponents(view);
        assert (retComp.length() < view.getComponentCount());

        for (int i = 0; i < view.getComponentCount(); i++) {
            if (!retComp.get(i)) {
                int original = view.getComponentMapping(i);
                components.clear(original);
            }
        }
    }
}
