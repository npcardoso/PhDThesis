package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;

/**
 * This class provides a way of creating a spectrum.
 */
public final class EditableSpectrum<A extends Activity,
                                      TM extends Metadata>
extends Spectrum<A, TM> {
    private final ArrayList<Transaction<A, TM> > transactions = new ArrayList();
    private final ArrayList<Component> components = new ArrayList();

    private final EditableTree tree = new EditableTree("root");
    private int componentCount = 0;

    @Override
    public EditableTree getTree () {
        return tree;
    }

    @Override
    public int getComponentCount () {
        return componentCount;
    }

    @Override
    public int getTransactionCount () {
        return transactions.size();
    }

    /**
     * @brief Retreives a transaction by id.
     * @return A transaction or null if a transaction with such id
     * does not exist.
     */
    @Override
    public Transaction<A, TM> getTransaction (int transactionId) {
        if (transactionId < 0 || transactionId >= transactions.size())
            return null;

        return transactions.get(transactionId);
    }


    /**
     * @brief Retreives a component by id.
     * @return A component or null if a component with such id does
     * not exist.
     */
    @Override
    public Component getComponent (int id) {
    	if(components.size() == 0) 
    		return null;
    	
        if (id < 0 || id >= components.size())
            return null;

        return components.get(id);
    }

    /**
     * @brief Retreives the list of components
     * @return A list of components
=     */
    @Override
    public ArrayList<Component> getComponents () {
        return components;
    }    
    
    /**
     * @brief Adds a new transactions to the spectrum.
     * @pre this.getTransaction(transaction.getId()) == null
     */
    public void setTransaction (Transaction<A, TM> transaction) {
        assert(this.getTransaction(transaction.getId()) == null);
        transactions.ensureCapacity(transaction.getId() + 1);

        while (transactions.size() <= transaction.getId()) {
            transactions.add(null);
        }

        transactions.set(transaction.getId(), transaction);

        componentCount = Math.max(componentCount, transaction.size());
    }

    /**
     * @throws NoComponentInformationAvailableException 
     * @brief Adds a new component to the spectrum.
     * @pre node.getTree() == this.getTree()
     * @pre this.getComponent(id) == null
     */
    public void setComponent (int id,
                              ProbeType type,
                              Node node) throws NoComponentInformationAvailableException {
        assert(node.getTree() == this.getTree());
        assert(this.getComponent(id) == null);
 components.ensureCapacity(id + 1);

        while (components.size() <= id){
            components.add(null);
        }

        components.set(id, new Component(this, type, id, node.getId()));
    }
}
