package io.crowbar.diagnostic.spectrum;

class TransactionView
extends Transaction {
    private final Transaction transaction;
    private final SpectrumView spectrumView;
    private final int active;
    private final int sze;

    TransactionView (int id,
                     SpectrumView spectrumView,
                     Transaction transaction) {
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

        for (Integer i : this) {
            active++;
        }

        this.active = active;
    }

    @Override
    public Transaction getOriginal () {
        return transaction;
    }

    @Override
    public final boolean isActive (int id) {
        return transaction.isActive(spectrumView.getProbeMapping(id));
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
    public int numActive () {
        return active;
    }

    @Override
    public int size () {
        return sze;
    }
}