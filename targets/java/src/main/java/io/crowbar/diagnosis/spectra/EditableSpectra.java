package io.crowbar.diagnosis.spectra;

import java.util.ArrayList;

public class EditableSpectra<A extends Activity,
                             TM extends Metadata,
                             CM extends Metadata>
extends Spectra<A, TM, CM> {
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

    public final void setTransaction (Transaction<A, TM> transaction) {
        transactions.ensureCapacity(transaction.getId() + 1);

        while (transactions.size() <= transaction.getId()) {
            transactions.add(null);
        }

        transactions.set(transaction.getId(), transaction);

        numComponents = Math.max(numComponents, transaction.size());
    }

    public final void setComponent (Component<CM> component) {
        components.ensureCapacity(component.getId() + 1);

        while (components.size() <= component.getId()) {
            components.add(null);
        }

        components.set(component.getId(), component);
    }
}