package io.crowbar.diagnosis.spectra;

import java.util.ArrayList;
import java.util.Iterator;

public class EditableSpectra<T extends Transaction, M> implements Spectra<T, M> {
    private final ArrayList<T> transactions = new ArrayList<T> ();
    private final ArrayList<M> metadata = new ArrayList<M> ();


    @Override
    public final int getNumComponents () {
        int numComponents = 0;


        for (T transaction : transactions) {
            numComponents = Math.max(numComponents, transaction.size());
        }

        return numComponents;
    }

    @Override
    public final int getNumTransactions () {
        return transactions.size();
    }

    @Override
    public final T getTransaction (int transactionId) {
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
    public final Iterator<T> iterator () {
        return transactions.iterator();
    }

    public final void appendTransaction (T transaction) {
        setTransaction(transactions.size(),
                       transaction);
    }

    public final void setTransaction (int transactionId,
                                      T transaction) {
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