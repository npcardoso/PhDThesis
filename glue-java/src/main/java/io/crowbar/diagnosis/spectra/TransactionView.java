package io.crowbar.diagnosis.spectra;

class TransactionView<A extends Activity,
                      TM extends Metadata>
extends Transaction<A, TM> {
    private final Transaction<A, TM> transaction;
    private final SpectraView<A, TM, ? > spectraView;
    private final int active;
    private final int sze;

    TransactionView (int id,
                     SpectraView<A, TM, ? > spectraView,
                     Transaction<A, TM> transaction) {
        super(id);
        this.spectraView = spectraView;
        this.transaction = transaction;
        int size;

        // compute correct size for transaction + view
        for (size = spectraView.getComponentCount(); size > 0; size--) {
            if (spectraView.getComponentMapping(size - 1) < transaction.size())
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
        return transaction.get(spectraView.getComponentMapping(id));
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
