package io.crowbar.diagnosis.spectra;

import io.crowbar.diagnosis.spectra.matcher.SpectraMatcher;
import java.util.BitSet;

public final  class SpectraViewFactory<A extends Activity,
                                       TM extends Metadata,
                                       CM extends Metadata> {
    private final Spectra<A, TM, CM> spectra;

    private final BitSet transactions = new BitSet();
    private final BitSet components = new BitSet();

    public SpectraViewFactory (Spectra<A, TM, CM> spectra) {
        this.spectra = spectra;
        transactions.set(0, spectra.getTransactionCount());
        components.set(0, spectra.getComponentCount());
    }

    public SpectraView<A, TM, CM> getView () {
        return new SpectraView<A, TM, CM> (this.spectra,
                                           this.transactions,
                                           this.components);
    }

    public void addStage (SpectraMatcher< ? super A, ? super TM, ? super CM> matcher) {
        SpectraView<A, TM, CM> view = getView();


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
