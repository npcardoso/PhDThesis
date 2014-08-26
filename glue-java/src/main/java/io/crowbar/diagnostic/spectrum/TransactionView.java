package io.crowbar.diagnostic.spectrum;

class TransactionView<A extends Activity,
                      TM extends Metadata>
extends Transaction<A, TM> {
    private final Transaction<A, TM> transaction;
    private final SpectrumView<A, TM> spectrumView;
    private final int active;
    private final int sze;

    TransactionView (int id,
                     SpectrumView<A, TM> spectrumView,
                     Transaction<A, TM> transaction) {
        super(id);
        this.spectrumView = spectrumView;
        this.transaction = transaction;
        int size;

        // compute correct size for transaction + view
        for (size = spectrumView.getProbeCount(); size > 0; size--) {
            if (spectrumView.getProbeMapping(size - 1) < transaction.size())
                break;
        }

        this.sze = size;

        // compute correct numActive for transaction + view
        int active = 0;

        for (A a : this) {
            if (a != null)
                active += a.isActive() ? 1 : 0;
        }

        this.active = active;
    }

    @Override
    public final A get (int id) {
        return transaction.get(spectrumView.getProbeMapping(id));
    }

    @Override
    public final double getError () {
        return transaction.getError();
    }

    @Override
    public double getConfidence () {
        return transaction.getConfidence();
    }

    @Override
    public TM getMetadata () {
        return transaction.getMetadata();
    }

    @Override
    public int numActive () {
        return active;
    }

    @Override
    public int size () {
        return sze;
    }
}