package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

public abstract class AbstractTree implements Tree {
    private ArrayList<Node> nodes = new ArrayList<Node> ();


    protected AbstractTree () {}

    public AbstractTree (String rootName) {
        try {
            addNode(new Node(rootName, 0, -1));
        }
        catch (Tree.RegistrationException e) {
            e.printStackTrace(); // ! Should never happen
        }
    }

    @Override
    public final int size () {
        return nodes.size();
    }

    @Override
    public final Node getRoot () {
        return getNode(0);
    }

    @Override
    public final Node getNode (int id) {
        if (id < 0 || id >= nodes.size())
            return null;

        return nodes.get(id);
    }

    @Override
    public final List<Node> getNodes () {
        return Collections.unmodifiableList(nodes);
    }

    @Override
    public final Iterator<Node> iterator () {
        return nodes.iterator();
    }

    /**
     * \brief Registers a node as a child of another node.
     * This method should be called in order to bind a node to a Tree.
     * The Node should be properly initialized.
     * The following actions are performed:
     *  - Add the child node to both parent's list and map.
     *  - Set the child's tree equal to "this".
     *  - Add the node to nodes list at correct position (extending the list
     * with null references if needed).
     * \throws AlreadyBoundException: if the node is already bound to some tree.
     * \throws AlreadyRegisteredException: if the a node with same id exists in the tree.
     * \throws InvalidRootNodeException: if the node is a root node but the tree already has one root node.
     * \throws NoSuchNodeException: if parent node does not exist and the node is not a root node.
     */
    protected final void addNode (Node node) throws Tree.RegistrationException {
        if (node.isBound())
            throw new AlreadyBoundException(node);

        Node existent = getNode(node.getId());

        if (existent != null) {
            throw new AlreadyRegisteredException(node, existent);
        }

        Node parent = getNode(node.getParentId());

        if (parent == null) {
            if (node.isRoot()) {
                if (nodes.size() != 0 || node.getId() != 0)
                    throw new InvalidRootNodeException(node);
            }
            else
                throw new NoSuchNodeException(node.getParentId());
        }
        else {
            parent.addChild(node.getName(), node.getId());
        }

        node.setTree(this);

        nodes.ensureCapacity(node.getId() + 1);

        while (nodes.size() <= node.getId()) {
            nodes.add(null);
        }

        nodes.set(node.getId(), node);
    }
}
