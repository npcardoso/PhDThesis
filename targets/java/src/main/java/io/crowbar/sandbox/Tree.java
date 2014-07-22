package io.crowbar.sandbox;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class Tree { // , Iterable<Tree> {
    public class Node implements java.io.Serializable {
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

        private Node (String name,
                      int id,
                      int parent_id) {
            this.name = name;
            this.id = id;
            this.parent_id = parent_id;
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

        public Node getParent () {
            return getNode(parent_id);
        }

        public Node getChild (String name) {
            Integer child_id = children_by_name.get(name);


            if (child_id == null)
                return null;

            return getNode(child_id);
        }

        public Node addChild (String name) {
            Node n = new Node(name, nodes.size(), getId());


            nodes.add(n);
            children.add(n.getId());
            children_by_name.put(n.getName(), n.getId());
            addChildHook(this, n);
            return n;
        }

        @Override
        public String toString () {
            return "[Node: \"" + getFullName() + "\"]";
        }
    }


    public Tree () {
        this("");
    }

    public Tree (String root_name) {
        nodes.add(new Node(root_name, 0, -1));
    }

    protected void addChildHook (Node parent,
                                 Node child) {}


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