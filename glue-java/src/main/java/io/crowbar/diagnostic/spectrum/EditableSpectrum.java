package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import flexjson.JSON;

/**
 * This class provides a way of creating a spectrum.
 */
public final class EditableSpectrum
extends Spectrum {
    private final ArrayList<Transaction> transactions = new ArrayList<Transaction> ();
    private final ArrayList<Probe> probes = new ArrayList<Probe> ();

    private final EditableTree tree = new EditableTree("root");
    private int probeCount = 0;

    @Override
    public EditableTree getTree () {
        return tree;
    }

    @JSON(include = false)
    @Override
    public int getTransactionCount () {
        return transactions.size();
    }

    @JSON(include = false)
    @Override
    public int getProbeCount () {
        return probeCount;
    }

    @Override
    public Transaction getTransaction (int transactionId) {
        if (transactionId < 0 || transactionId >= transactions.size())
            return null;

        return transactions.get(transactionId);
    }

    @Override
    public Probe getProbe (int id) {
        if (id < 0 || id >= probes.size())
            return null;

        return probes.get(id);
    }

    /**
     * @brief Adds a new transactions to the spectrum.
     */
    public void setTransaction (Transaction transaction) {
        transactions.ensureCapacity(transaction.getId() + 1);

        while (transactions.size() <= transaction.getId()) {
            transactions.add(null);
        }

        transactions.set(transaction.getId(), transaction);

        probeCount = Math.max(probeCount, transaction.size());
    }

    /**
     * @brief Adds a new probe to the spectrum.
     * @pre node.getTree() == this.getTree()
     */
    public void setProbe (int id,
                          ProbeType type,
                          Node node) {
        assert (node.getTree() == this.getTree());
        probes.ensureCapacity(id + 1);

        while (probes.size() <= id) {
            probes.add(null);
        }

        probeCount = Math.max(probeCount, id + 1);
        probes.set(id, new Probe(this, type, id, node.getId()));
    }
}