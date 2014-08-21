package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import flexjson.JSON;

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
    @JSON(include=false)
    public boolean isBound () {
        return tree != null;
    }

    @JSON(include=false)
    public boolean isRoot () {
        return parentId < 0;
    }

    @JSON(include=false)
    public int getId () {
        return id;
    }

    @JSON(name="n")
    public String getName () {
        return name;
    }

    /*!
     * \brief Concatenates the names of all nodes in the path to the root using ":" as separator.
     */
    @JSON(include=false)
    public String getFullName () {
        return getFullName(":");
    }

    /*!
     * \brief Concatenates the names of all nodes in the path to the root using "separator" as separator.
     */
    @JSON(include=false)
    public String getFullName (String separator) {
        return getFullName(separator, 0);
    }

    /*!
     * \brief Concatenates the names of all nodes in the path to the root using "separator" as separator.
     */
    @JSON(include=false)
    public String getFullName (String separator,
                               int fromDepth) {
        Node p = getParent();


        if (p == null || getDepth() <= fromDepth)
            return name;

        return p.getFullName(separator, fromDepth) + separator + name;
    }

    @JSON(name="p")
    public int getParentId () {
        return parentId;
    }

    @JSON(include=false)
    public Node getParent () {
        return getNode(parentId);
    }

    @JSON(include=false)
    public int getDepth () {
        return depth;
    }

    @JSON(include=false)
    public List<Integer> getChildren () {
        return Collections.unmodifiableList(children);
    }

    @JSON(include=false)
    public Node getChild (String name) {
        Integer childId = childrenByName.get(name);


        if (childId == null)
            return null;

        return getNode(childId);
    }

    @JSON(include=false)
    private Tree getTree() {
        return tree;
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

    @JSON(include=false)
    private Node getNode (int nodeId) {
        assert tree != null; // ! @TODO: Create proper exception
        return tree.getNode(nodeId);
    }

    @JSON(include=false)
    void addChild (String name,
                   int nodeId) {
        children.add(nodeId);
        childrenByName.put(name, nodeId);
    }

    @JSON(include=false)
    void setTree (Tree tree) {
        this.tree = tree;
        Node n = getNode(parentId);

        if (n == null)
            depth = 0;
        else
            depth = n.depth + 1;
    }
}
