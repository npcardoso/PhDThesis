package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;

/**
 * This class provides a way of creating a spectrum.
 * \return
 * \retval
 */
public class EditableSpectrum<A extends Activity,
                             TM extends Metadata,
                             CM extends Metadata>
extends Spectrum<A, TM, CM> {
    private final ArrayList<Transaction<A, TM> > transactions = new ArrayList();
    private final ArrayList<Component<CM> > components = new ArrayList();

    private int componentCount = 0;

    @Override
    public final int getComponentCount () {
        return componentCount;
    }

    @Override
    public final int getTransactionCount () {
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

    /**
     * \brief Adds a new transactions to the spectrum.
     *
     * \note If a transaction with the same id already exists, the old
     * transaction is replaced.
     */

    public final void setTransaction (Transaction<A, TM> transaction) {
        transactions.ensureCapacity(transaction.getId() + 1);

        while (transactions.size() <= transaction.getId()) {
            transactions.add(null);
        }

        transactions.set(transaction.getId(), transaction);

        componentCount = Math.max(componentCount, transaction.size());
    }

    /**
     * \brief Adds a new component to the spectrum.
     *
     * \note If a component with the same id already exists, the old
     * component is replaced.
     */
    public final void setComponent (Component<CM> component) {
        components.ensureCapacity(component.getId() + 1);

        while (components.size() <= component.getId()) {
            components.add(null);
        }

        components.set(component.getId(), component);
    }
}
