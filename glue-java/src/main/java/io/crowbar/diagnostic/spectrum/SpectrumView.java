package io.crowbar.diagnostic.spectrum;

import java.util.BitSet;

/*!
 * This class creates a view of an arbitrary spectrum with some components/transactions ommited.
 * As this class does not make any copies of the original spectrum, it assumes its immutability.
 * Changes to the spectrum after the creation of the view may have unpredictable consequences.
 */
final class SpectrumView<A extends Activity,
                        TM extends Metadata,
                        CM extends Metadata>
extends Spectrum<A, TM, CM> {
    private final Spectrum<A, TM, CM> spectrum;
    private final int[] components;
    private final int[] transactions;

    public SpectrumView (Spectrum<A, TM, CM> spectrum,
                        BitSet transactions,
                        BitSet components) {
        this.spectrum = spectrum;
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
    public int getComponentCount () {
        return components.length;
    }

    @Override
    public int getTransactionCount () {
        return transactions.length;
    }

    @Override
    public Transaction<A, TM> getTransaction (int transactionId) {
        Transaction<A, TM> t = spectrum.getTransaction(transactions[transactionId]);

        if (t == null)
            return null;

        return new TransactionView<A, TM> (transactionId, this, t);
    }

    @Override
    public Component<CM> getComponent (int componentId) {
        return spectrum.getComponent(components[componentId]);
    }
}
