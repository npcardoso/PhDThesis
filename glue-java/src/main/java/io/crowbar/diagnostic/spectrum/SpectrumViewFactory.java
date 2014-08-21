package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.spectrum.matcher.SpectrumMatcher;
import java.util.BitSet;

public final  class SpectrumViewFactory<A extends Activity,
                                       TM extends Metadata,
                                       CM extends Metadata> {
    private final Spectrum<A, TM, CM> spectrum;

    private final BitSet transactions = new BitSet();
    private final BitSet components = new BitSet();

    public SpectrumViewFactory (Spectrum<A, TM, CM> spectrum) {
        this.spectrum = spectrum;
        transactions.set(0, spectrum.getTransactionCount());
        components.set(0, spectrum.getComponentCount());
    }

    public SpectrumView<A, TM, CM> getView () {
        return new SpectrumView<A, TM, CM> (this.spectrum,
                                           this.transactions,
                                           this.components);
    }

    public void addStage (SpectrumMatcher< ? super A, ? super TM, ? super CM> matcher) {
        SpectrumView<A, TM, CM> view = getView();


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
