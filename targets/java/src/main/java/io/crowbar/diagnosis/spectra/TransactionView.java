package io.crowbar.diagnosis.spectra;

import java.util.Iterator;
import java.util.NoSuchElementException;

class TransactionView<A extends Activity> implements Transaction<A> {
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

    private final Transaction<A> transaction;
    private final SpectraView<A, ? > spectraView;

    TransactionView (SpectraView<A, ? > spectraView,
                     Transaction<A> transaction) {
        this.spectraView = spectraView;
        this.transaction = transaction;
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
    public Iterator<A> iterator () {
        System.out.println("Creating TransactionIterator");

        return new TransactionIterator();
    }

    @Override
    public int numActive () {
        return 0; // TODO
    }

    @Override
    public int size () {
        return spectraView.getNumComponents(); // TODO: this is not 100% accurate
    }
}