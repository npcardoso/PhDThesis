package io.crowbar.instrumentation.runtime;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class Tree { // , Iterable<Tree> {
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
        private Map<String, Integer> children_by_name = children_by_name = new HashMap<String, Integer> ();
        /*!
         * \brief A map for additional node properties.
         */
        private Map<String, String> properties = null;

        private Node (Tree tree,
                      String name,
                      int id,
                      int parent_id) {
            this.tree = tree;
            this.name = name;
            this.id = id;
            this.parent_id = parent_id;
        }

        /*!
         * \brief Checks if node is bound to a tree.
         */
        public boolean isBound () {
            return tree != null;
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
                return getName();

            return p.getFullName(separator) + separator + name;
        }

        public int getParentId () {
            return parent_id;
        }

        public Node getParent () {
            return getNode(parent_id);
        }

        public Node getChild (String name) {
            Integer child_id = children_by_name.get(name);


            if (child_id == null)
                return null;

            return getNode(child_id);
        }

        private Node getNode (int node_id) {
            assert tree != null; // ! @TODO: Create proper exception
            return tree.getNode(node_id);
        }

        public Node addChild (String name) {
            assert tree != null; // ! @TODO: Create proper exception
            Node n = new Node(tree, name, tree.nodes.size(), getId());


            tree.nodes.add(n);
            children.add(n.getId());
            children_by_name.put(n.getName(), n.getId());
            tree.addChildHook(n);
            return n;
        }

        @Override
        public String toString () {
            String ret = "[state: " + (isBound() ? "B" : "Unb") + "ound, ";


            ret += "name: \"" + (isBound() ? getFullName() : getName()) + "\", ";
            ret += "id: " + getId() + ", ";
            ret += "parent_id: " + getParentId() + "]";
            return ret;
        }
    }


    public Tree () {
        this("");
    }

    public Tree (String root_name) {
        nodes.add(new Node(this, root_name, 0, -1));
    }

    protected void addChildHook (Node child) {}


    public Node getRoot () {
        return nodes.get(0);
    }

    public Node getNode (int id) {
        if (id < 0 || id >= nodes.size())
            return null;

        return nodes.get(id);
    }

    private List<Node> nodes = new ArrayList<Node> ();
}