package io.crowbar.instrumentation.runtime;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public final class Node implements java.io.Serializable {
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
     * \brief The node's depth in the tree.
     */
    private int depth = -1;
    /*!
     * \brief The node's children ids.
     */
    private final List<Integer> children = new ArrayList<Integer> ();
    /*!
     * \brief The node's children ids, acessible by name.
     */
    private final Map<String, Integer> childrenByName = new HashMap<String, Integer> ();

    Node (String name,
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
        return getFullName(separator, 0);
    }

    /*!
     * \brief Concatenates the names of all nodes in the path to the root using "separator" as separator.
     */
    public String getFullName (String separator,
                               int fromDepth) {
        Node p = getParent();


        if (p == null || getDepth() <= fromDepth)
            return name;

        return p.getFullName(separator, fromDepth) + separator + name;
    }

    public int getParentId () {
        return parentId;
    }

    public Node getParent () {
        return getNode(parentId);
    }

    public int getDepth () {
        return depth;
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
        ret += "parentId: " + getParentId() + ", ";
        ret += "children: " + children + "]";
        return ret;
    }

    private Node getNode (int nodeId) {
        assert tree != null; // ! @TODO: Create proper exception
        return tree.getNode(nodeId);
    }

    void addChild (String name,
                   int nodeId) {
        children.add(nodeId);
        childrenByName.put(name, nodeId);
    }

    void setTree (Tree tree) {
        this.tree = tree;
        Node n = getNode(parentId);

        if (n == null)
            depth = 0;
        else
            depth = n.depth + 1;
    }
}