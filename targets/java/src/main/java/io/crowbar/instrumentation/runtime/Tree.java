package io.crowbar.instrumentation.runtime;


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
        public AlreadyRegisteredException (Node node, Node existent_node) {
            super("Node with node_id: " + node.getId() + " already exists. " +
                  "New: " + node + ", " +
                  "Existent: " + existent_node);
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

    public static class Node implements java.io.Serializable {
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
        private String name;
        /*!
         * \brief The node's id.
         */
        private int id;
        /*!
         * \brief The node's parent id.
         */
        private int parent_id;
        /*!
         * \brief The node's children ids.
         */
        private List<Integer> children = new ArrayList<Integer> ();
        /*!
         * \brief The node's children ids, acessible by name.
         */
        private Map<String, Integer> children_by_name = new HashMap<String, Integer> ();
        /*!
         * \brief A map for additional node properties.
         */
        private Map<String, String> properties = null;

        private Node (String name,
                      int id,
                      int parent_id) {
            this.name = name;
            this.id = id;
            this.parent_id = parent_id;
        }

        public Node (Node n) {
            this.name = n.name;
            this.id = n.id;
            this.parent_id = n.parent_id;
        }

        /*!
         * \brief Checks if node is bound to a tree.
         */
        public boolean isBound () {
            return tree != null;
        }

        public boolean isRoot () {
            return parent_id < 0;
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
            return parent_id;
        }

        public Node getParent () {
            return getNode(parent_id);
        }

        public List<Integer> getChildren () {
            return Collections.unmodifiableList(children);
        }

        public Node getChild (String name) {
            Integer child_id = children_by_name.get(name);


            if (child_id == null)
                return null;

            return getNode(child_id);
        }

        public Node addChild (String name) throws RegistrationException {
            assert tree != null; // ! @TODO: Create proper exception
            Node n = new Node(name, tree.nodes.size(), this.id);
            tree.registerChild(n);
            return n;
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

        private Node getNode (int node_id) {
            assert tree != null; // ! @TODO: Create proper exception
            return tree.getNode(node_id);
        }
    }


    protected Tree () {}

    public Tree (String root_name) {
        try {
            registerChild(new Node(root_name, 0, -1));
        }
        catch (RegistrationException e) {
            e.printStackTrace(); // ! Should never happen
        }
    }

    public Node getRoot () {
        return getNode(0);
    }

    public Node getNode (int id) {
        if (id < 0 || id >= nodes.size())
            return null;

        return nodes.get(id);
    }

    public List<Node> getNodes () {
        return Collections.unmodifiableList(nodes);
    }

    public Iterator<Node> iterator () {
        return nodes.iterator();
    }

    protected void registerChild (Node node) throws RegistrationException {
        assert node.getId() == nodes.size();
        _registerChild(node);
        nodes.add(node);
    }

    /*!
     * \brief Registers a node as a child of another node.
     * This method should be called in order to bind a node to a Tree.
     * The Node should be properly initialized.
     * The following actions are performed:
     *  - Add the child node to both parent's list and map.
     *  - Set the child's tree equal to "this".
     * The following action is *not* performed:
     *  - Add the node to nodes list at correct position.
     * The node should be added to the list *after* calling this method.
     * @throws AlreadyBoundException: if the node is already bound to some tree.
     * @throws AlreadyRegisteredException: if the a node with same id exists in the tree.
     * @throws InvalidRootNodeException: if the node is a root node but the tree already has one root node.
     * @throws NoSuchNodeException: if parent node does not exist and the node is not a root node.
     */
    protected final void _registerChild (Node node) throws RegistrationException {
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
            parent.children_by_name.put(node.getName(), node.getId());
        }

        node.tree = this;
    }

    protected ArrayList<Node> nodes = new ArrayList<Node> ();
}