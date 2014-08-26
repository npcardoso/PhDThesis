package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.BitSet;

/*!
 * This class creates a view of an arbitrary spectrum with some probes/transactions ommited.
 * As this class does not make any copies of the original spectrum, it assumes its immutability.
 * Changes to the spectrum after the creation of the view may have unpredictable consequences.
 */
final class SpectrumView<A extends Activity,
                         TM extends Metadata>
extends Spectrum<A, TM> {
    private final Spectrum<A, TM> spectrum;
    private final int[] probes;
    private final int[] transactions;

    public SpectrumView (Spectrum<A, TM> spectrum,
                         BitSet transactions,
                         BitSet probes) {
        this.spectrum = spectrum;
        this.transactions = toArray(transactions);
        this.probes = toArray(probes);
    }

    private static int[] toArray (BitSet bs) {
        int[] ret = new int[bs.cardinality()];
        int i = 0;

        for (int el = bs.nextSetBit(0); el >= 0; el = bs.nextSetBit(el + 1)) {
            ret[i++] = el;
        }

        return ret;
    }

    @Override
    public Tree getTree () {
        return spectrum.getTree();
    }

    public int getProbeMapping (int id) {
        return probes[id];
    }

    public int getTransactionMapping (int id) {
        return transactions[id];
    }

    @Override
    public int getProbeCount () {
        return probes.length;
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
    public Probe getProbe (int probeId) {
        return spectrum.getProbe(probes[probeId]);
    }

    @Override
    public ArrayList<Probe> getProbes () {
        return spectrum.getProbes();
    }
}