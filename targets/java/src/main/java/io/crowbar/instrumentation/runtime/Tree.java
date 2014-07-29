package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class Tree implements Iterable<Tree.Node> {
    public static class RegistrationException extends Exception {
        public RegistrationException (String str) {
            super(str);
        }
    }

    public static class AlreadyBoundException extends RegistrationException {
        public AlreadyBoundException (Node node) {
            super("Node " + node + " is already bound.");
        }
    }

    public static class AlreadyRegisteredException extends RegistrationException {
        public AlreadyRegisteredException (Node node, Node existentNode) {
            super("Node with node_id: " + node.getId() + " already exists. " +
                  "New: " + node + ", " +
                  "Existent: " + existentNode);
        }
    }

    public static class InvalidRootNodeException extends RegistrationException {
        public InvalidRootNodeException (Node node) {
            super("Trying to register root node " + node +
                  " in a tree that already contains a node.");
        }
    }

    public static class NoSuchNodeException extends RegistrationException {
        public NoSuchNodeException (int id) {
            super("Node with node_id: " + id + " does not exist!");
        }
    }

    public static final class Node implements java.io.Serializable {
        /*!
         * \brief The tree holding the node
         * This field is marked as transient for serialization purposes.
         */
        private transient Tree tree = null;
        /*!
         * \brief The node's name.
         * If the node represents:
         *  - A class: class name.
         *  - A method: method name
         *  - A line: line number
         */
        private final String name;
        /*!
         * \brief The node's id.
         */
        private final int id;
        /*!
         * \brief The node's parent id.
         */
        private final int parentId;
        /*!
         * \brief The node's children ids.
         */
        private final List<Integer> children = new ArrayList<Integer> ();
        /*!
         * \brief The node's children ids, acessible by name.
         */
        private final Map<String, Integer> childrenByName = new HashMap<String, Integer> ();
        /*!
         * \brief A map for additional node properties.
         */
        private Map<String, String> properties = null;

        private Node (String name,
                      int id,
                      int parentId) {
            this.name = name;
            this.id = id;
            this.parentId = parentId;
        }

        public Node (Node n) {
            this.name = n.name;
            this.id = n.id;
            this.parentId = n.parentId;
        }

        /*!
         * \brief Checks if node is bound to a tree.
         */
        public boolean isBound () {
            return tree != null;
        }

        public boolean isRoot () {
            return parentId < 0;
        }

        public int getId () {
            return id;
        }

        public String getName () {
            return name;
        }

        /*!
         * \brief Concatenates the names of all nodes in the path to the root using ":" as separator.
         */
        public String getFullName () {
            return getFullName(":");
        }

        /*!
         * \brief Concatenates the names of all nodes in the path to the root using "separator" as separator.
         */
        public String getFullName (String separator) {
            Node p = getParent();


            if (p == null)
                return name;

            return p.getFullName(separator) + separator + name;
        }

        public int getParentId () {
            return parentId;
        }

        public Node getParent () {
            return getNode(parentId);
        }

        public List<Integer> getChildren () {
            return Collections.unmodifiableList(children);
        }

        public Node getChild (String name) {
            Integer childId = childrenByName.get(name);


            if (childId == null)
                return null;

            return getNode(childId);
        }

        @Override
        public String toString () {
            String ret = "[state: " + (isBound() ? "B" : "Unb") + "ound, ";


            ret += "name: \"" + (isBound() ? getFullName() : getName()) + "\", ";
            ret += "id: " + getId() + ", ";
            ret += "parent_id: " + getParentId() + ", ";
            ret += "children: " + children + "]";
            return ret;
        }

        private Node getNode (int nodeId) {
            assert tree != null; // ! @TODO: Create proper exception
            return tree.getNode(nodeId);
        }
    }

    private ArrayList<Node> nodes = new ArrayList<Node> ();


    protected Tree () {}

    public Tree (String rootName) {
        try {
            addNode(new Node(rootName, 0, -1));
        }
        catch (RegistrationException e) {
            e.printStackTrace(); // ! Should never happen
        }
    }

    public final Node getRoot () {
        return getNode(0);
    }

    public final Node getNode (int id) {
        if (id < 0 || id >= nodes.size())
            return null;

        return nodes.get(id);
    }

    public final List<Node> getNodes () {
        return Collections.unmodifiableList(nodes);
    }

    public final Iterator<Node> iterator () {
        return nodes.iterator();
    }

    public final Node addNode (String name,
                               Node parent) throws RegistrationException {
        Node n = new Node(name,
                          nodes.size(),
                          parent.getId());


        addNode(n);
        return n;
    }

    /*!
     * \brief Registers a node as a child of another node.
     * This method should be called in order to bind a node to a Tree.
     * The Node should be properly initialized.
     * The following actions are performed:
     *  - Add the child node to both parent's list and map.
     *  - Set the child's tree equal to "this".
     *  - Add the node to nodes list at correct position.
     * @throws AlreadyBoundException: if the node is already bound to some tree.
     * @throws AlreadyRegisteredException: if the a node with same id exists in the tree.
     * @throws InvalidRootNodeException: if the node is a root node but the tree already has one root node.
     * @throws NoSuchNodeException: if parent node does not exist and the node is not a root node.
     */
    protected final void addNode (Node node) throws RegistrationException {
        if (node.tree != null)
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
            parent.children.add(node.getId());
            parent.childrenByName.put(node.getName(), node.getId());
        }

        node.tree = this;

        nodes.ensureCapacity(node.getId() + 1);

        while (nodes.size() <= node.getId()) {
            nodes.add(null);
        }

        nodes.set(node.getId(), node);
    }
}