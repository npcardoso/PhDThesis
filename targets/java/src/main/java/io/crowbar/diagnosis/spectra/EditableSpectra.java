package io.crowbar.diagnosis.spectra;

import java.util.ArrayList;
import java.util.Iterator;

public class EditableSpectra<A extends Activity,
                             TM extends Metadata,
                             CM extends Metadata>
implements Spectra<A, TM, CM> {
    private final ArrayList<Transaction<A, TM> > transactions = new ArrayList();
    private final ArrayList<Component<CM> > components = new ArrayList();

    private int numComponents = 0;

    @Override
    public final int getNumComponents () {
        return numComponents;
    }

    @Override
    public final int getNumTransactions () {
        return transactions.size();
    }

    @Override
    public final Transaction<A, TM> getTransaction (int transactionId) {
        if (transactionId < 0 || transactionId >= transactions.size())
            return null;

        return transactions.get(transactionId);
    }

    @Override
    public final Component<CM> getComponent (int componentId) {
        if (componentId < 0 || componentId >= components.size())
            return null;

        return components.get(componentId);
    }

    @Override
    public final Iterator<Transaction<A, TM> > iterator () {
        return transactions.iterator();
    }

    public final void appendTransaction (Transaction<A, TM> transaction) {
        setTransaction(transactions.size(),
                       transaction);
    }

    public final void setTransaction (int transactionId,
                                      Transaction<A, TM> transaction) {
        transactions.ensureCapacity(transactionId + 1);

        while (transactions.size() <= transactionId) {
            transactions.add(null);
        }

        transactions.set(transactionId, transaction);

        numComponents = Math.max(numComponents, transaction.size());
    }

    public final void setComponent (int componentId,
                                    Component<CM> metadata) {
        components.ensureCapacity(componentId + 1);

        while (components.size() <= componentId) {
            components.add(null);
        }

        components.set(componentId, metadata);
    }
}