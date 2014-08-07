package io.crowbar.diagnosis.spectra;

import java.util.Iterator;
import java.util.NoSuchElementException;

class TransactionView<A extends Activity,
                      TM extends Metadata>
implements Transaction<A, TM> {
    private class TransactionIterator implements Iterator<A> {
        private int i = 0;

        @Override
        public boolean hasNext () {
            return i < spectraView.getNumComponents();
        }

        @Override
        public A next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return get(i++);
        }

        @Override
        public void remove () {
            throw new UnsupportedOperationException();
        }
    };

    private final Transaction<A, TM> transaction;
    private final SpectraView<A, TM, ? > spectraView;
    private final int active;
    private final int sze;

    TransactionView (SpectraView<A, TM, ? > spectraView,
                     Transaction<A, TM> transaction) {
        this.spectraView = spectraView;
        this.transaction = transaction;
        int size;

        // compute correct size for transaction + view
        for (size = spectraView.getNumComponents(); size > 0; size--) {
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
    public Iterator<A> iterator () {
        return new TransactionIterator();
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