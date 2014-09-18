package io.crowbar.diagnostic.spectrum;

import io.crowbar.util.ViewUtils;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

/*!
 * This class creates a view of an arbitrary spectrum with some probes/transactions ommited.
 * As this class does not make any copies of the original spectrum, it assumes its immutability.
 * Changes to the spectrum after the creation of the view may have unpredictable consequences.
 */
final class SpectrumView
extends Spectrum {
    private final Spectrum spectrum;
    private final int[] probes;
    private final int[] transactions;

    private List<Probe> probeViews;

    public SpectrumView (Spectrum spectrum,
                         BitSet transactions,
                         BitSet probes) {
        this.spectrum = spectrum;
        this.transactions = ViewUtils.toMappingArray(transactions);
        this.probes = ViewUtils.toMappingArray(probes);

        this.probeViews = new ArrayList<Probe> (this.probes.length);

        for (int i = 0; i < this.probes.length; i++) {
            this.probeViews.add(null);
        }
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
    public Transaction getTransaction (int transactionId) {
        Transaction t = spectrum.getTransaction(transactions[transactionId]);


        if (t == null)
            return null;

        return new TransactionView(transactionId, this, t);
    }

    @Override
    public Probe getProbe (int probeId) {
        Probe p = probeViews.get(probeId);


        if (p != null)
            return p;

        p = spectrum.getProbe(probes[probeId]);

        if (p == null)
            return null;

        p = new Probe(spectrum, p.getType(), probeId, p.getNodeId());
        probeViews.set(probeId, p);

        return p;
    }

    @Override
    public List<Probe> getProbes () {
        return spectrum.getProbes();
    }
}