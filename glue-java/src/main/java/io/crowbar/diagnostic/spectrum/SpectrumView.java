package io.crowbar.diagnostic.spectrum;

import io.crowbar.util.ViewUtils;

import java.util.BitSet;
import java.util.List;

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
        this.transactions = ViewUtils.toMappingArray(transactions);
        this.probes = ViewUtils.toMappingArray(probes);
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
        Probe p = spectrum.getProbe(probes[probeId]);


        if (p == null)
            return null;

        return new Probe(spectrum, p.getType(), probeId, p.getNodeId());
    }

    @Override
    public List<Probe> getProbes () {
        return spectrum.getProbes();
    }
}