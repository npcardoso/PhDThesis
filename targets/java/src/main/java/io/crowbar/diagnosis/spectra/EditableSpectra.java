package io.crowbar.diagnosis.spectra;

import java.util.ArrayList;
import java.util.Iterator;

public class EditableSpectra<A extends Activity, M> implements Spectra<A, M> {
    private final ArrayList<Transaction<A> > transactions = new ArrayList<Transaction<A> > ();
    private final ArrayList<M> metadata = new ArrayList<M> ();


    @Override
    public final int getNumComponents () {
        int numComponents = 0;


        for (Transaction<A> transaction : transactions) {
            numComponents = Math.max(numComponents, transaction.size());
        }

        return numComponents;
    }

    @Override
    public final int getNumTransactions () {
        return transactions.size();
    }

    @Override
    public final Transaction<A> getTransaction (int transactionId) {
        if (transactionId < 0 || transactionId >= transactions.size())
            return null;

        return transactions.get(transactionId);
    }

    @Override
    public final M getMetadata (int componentId) {
        if (componentId < 0 || componentId >= metadata.size())
            return null;

        return metadata.get(componentId);
    }

    @Override
    public final Iterator<Transaction<A> > iterator () {
        return transactions.iterator();
    }

    public final void appendTransaction (Transaction<A> transaction) {
        setTransaction(transactions.size(),
                       transaction);
    }

    public final void setTransaction (int transactionId,
                                      Transaction<A> transaction) {
        transactions.ensureCapacity(transactionId + 1);

        while (transactions.size() <= transactionId) {
            transactions.add(null);
        }

        transactions.set(transactionId, transaction);
    }

    public final void setMetadata (int componentId,
                                   M m) {
        metadata.ensureCapacity(componentId + 1);

        while (metadata.size() <= componentId) {
            metadata.add(null);
        }

        metadata.set(componentId, m);
    }
}