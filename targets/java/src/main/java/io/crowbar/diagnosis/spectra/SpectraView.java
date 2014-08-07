package io.crowbar.diagnosis.spectra;

import io.crowbar.diagnosis.spectra.matcher.SpectraMatcher;

import java.util.SortedSet;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.List;
import java.util.LinkedList;

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
    };

    private final Spectra<A, TM, CM> spectra;
    private final int[] components;
    private final int[] transactions;

    public SpectraView (Spectra<A, TM, CM> spectra,
                        SpectraMatcher< ? super A, ? super TM, ? super CM> matcher) {
        List<Integer> tmp;

        this.spectra = spectra;

        tmp = new LinkedList<Integer> ();

        for (int c = 0; c < spectra.getNumComponents(); c++) {
            if (matcher.matchComponent(spectra, c))
                tmp.add(c);
        }

        this.components = toArray(tmp);


        tmp = new LinkedList<Integer> ();

        for (int t = 0; t < spectra.getNumTransactions(); t++) {
            if (matcher.matchTransaction(spectra, t))
                tmp.add(t);
        }

        this.transactions = toArray(tmp);
        System.out.printf("View has %d tr, %d comp\n", getNumTransactions(), getNumComponents());
    }

    private static final int[] toArray (List<Integer> l) {
        int[] ret = new int[l.size()];
        int i = 0;

        for (Integer el : l) {
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