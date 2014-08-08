package io.crowbar.diagnosis.spectra;

import io.crowbar.diagnosis.spectra.matcher.SpectraMatcher;

import java.util.SortedSet;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.List;
import java.util.LinkedList;
import java.util.BitSet;

/*!
 * This class creates a view of an arbitrary spectra with some components/transactions ommited.
 * As this class does not make any copies of the original spectra, it assumes its immutability.
 * Changes to the spectra after the creation of the view may have unpredictable consequences.
 */
public class SpectraView<A extends Activity,
                         TM extends Metadata,
                         CM extends Metadata>
implements Spectra<A, TM, CM> {
    private class SpectraIterator implements Iterator<Transaction<A, TM> > {
        private int i = 0;

        @Override
        public boolean hasNext () {
            return i < getNumTransactions();
        }

        @Override
        public Transaction<A, TM> next () {
            if (!hasNext())
                throw new NoSuchElementException();

            return getTransaction(i++);
        }

        @Override
        public void remove () {
            throw new UnsupportedOperationException();
        }
    }

    private final Spectra<A, TM, CM> spectra;
    private final int[] components;
    private final int[] transactions;

    public SpectraView (Spectra<A, TM, CM> spectra,
                        BitSet transactions,
                        BitSet components) {
        this.spectra = spectra;
        this.transactions = toArray(transactions);
        this.components = toArray(components);
    }

    private static final int[] toArray (BitSet bs) {
        int[] ret = new int[bs.cardinality()];
        int i = 0;

        for (int el = bs.nextSetBit(0); el >= 0; el = bs.nextSetBit(el + 1)) {
            ret[i++] = el;
        }

        return ret;
    }

    public final int getComponentMapping (int id) {
        return components[id];
    }

    public final int getTransactionMapping (int id) {
        return transactions[id];
    }

    @Override
    public final int getNumComponents () {
        return components.length;
    }

    @Override
    public final int getNumTransactions () {
        return transactions.length;
    }

    @Override
    public final Transaction<A, TM> getTransaction (int transactionId) {
        Transaction<A, TM> t = spectra.getTransaction(transactions[transactionId]);

        if (t == null)
            return null;

        return new TransactionView<A, TM> (this, t);
    }

    @Override
    public final Component<CM> getComponent (int componentId) {
        return spectra.getComponent(components[componentId]);
    }

    @Override
    public final Iterator<Transaction<A, TM> > iterator () {
        return new SpectraIterator();
    }
}