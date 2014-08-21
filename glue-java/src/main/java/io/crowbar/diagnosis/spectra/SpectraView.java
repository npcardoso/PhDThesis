package io.crowbar.diagnosis.spectra;

import java.util.BitSet;

/*!
 * This class creates a view of an arbitrary spectra with some components/transactions ommited.
 * As this class does not make any copies of the original spectra, it assumes its immutability.
 * Changes to the spectra after the creation of the view may have unpredictable consequences.
 */
final class SpectraView<A extends Activity,
                        TM extends Metadata,
                        CM extends Metadata>
extends Spectra<A, TM, CM> {
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

    private static int[] toArray (BitSet bs) {
        int[] ret = new int[bs.cardinality()];
        int i = 0;

        for (int el = bs.nextSetBit(0); el >= 0; el = bs.nextSetBit(el + 1)) {
            ret[i++] = el;
        }

        return ret;
    }

    public int getComponentMapping (int id) {
        return components[id];
    }

    public int getTransactionMapping (int id) {
        return transactions[id];
    }

    @Override
    public int getNumComponents () {
        return components.length;
    }

    @Override
    public int getNumTransactions () {
        return transactions.length;
    }

    @Override
    public Transaction<A, TM> getTransaction (int transactionId) {
        Transaction<A, TM> t = spectra.getTransaction(transactions[transactionId]);

        if (t == null)
            return null;

        return new TransactionView<A, TM> (transactionId, this, t);
    }

    @Override
    public Component<CM> getComponent (int componentId) {
        return spectra.getComponent(components[componentId]);
    }
}
