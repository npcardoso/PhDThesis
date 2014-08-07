package io.crowbar.diagnosis.spectra;

import java.util.SortedSet;
import java.util.Iterator;
import java.util.NoSuchElementException;
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
                        SortedSet<Integer> components,
                        SortedSet<Integer> transactions) {
        int i;


        this.spectra = spectra;

        this.components = new int[components.size()];
        i = 0;

        for (int c : components) {
            assert c >= 0 && c < spectra.getNumComponents();
            this.components[i++] = c;
        }

        this.transactions = new int[transactions.size()];
        i = 0;

        for (int t : transactions) {
            assert t >= 0 && t < spectra.getNumTransactions();
            this.transactions[i++] = t;
        }
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
        return new TransactionView(this, spectra.getTransaction(transactions[transactionId]));
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