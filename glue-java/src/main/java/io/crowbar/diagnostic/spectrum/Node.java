package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import flexjson.JSON;

public class Node {
    private Tree tree;
    private String name;
    private int id;
    private int depth;
    private Node parent;
    private List<Node> children = new ArrayList<Node> ();
    private Map<String, Node> childrenByName = new HashMap<String, Node> ();

    Node () {}

    Node (Tree tree,
          String name,
          int id,
          Node parent) {
        this.tree = tree;
        this.name = name;
        this.id = id;
        this.parent = parent;

        if (isRoot())
            this.depth = 0;
        else {
            this.depth = parent.getDepth() + 1;
            parent.addChild(this);
        }
    }

    @JSON(include = false)
    public boolean isRoot () {
        return parent == null;
    }

    @JSON(include = false)
    public int getId () {
        return id;
    }

    @JSON(include = false)
    public int getPermanentId () {
        return id;
    }

    @JSON(name = "n")
    public String getName () {
        return name;
    }

    @JSON(name = "p")
    public int getParentId () {
        if (isRoot())
            return -1;

        return parent.getId();
    }

    /**
     * @brief Concatenates the names of all nodes in the path from the
     * root to this node using ":" as separator.
     */
    @JSON(include = false)
    public String getFullName () {
        return getFullName(":");
    }

    /**
     * @brief Concatenates the names of all nodes in the path from the
     * root to this node using "separator" as separator.
     */
    @JSON(include = false)
    public String getFullName (String separator) {
        return getFullName(separator, 0);
    }

    /**
     * @brief Concatenates the names of all nodes with depth greater
     * than "fromDepth" in the path from the root to this node using
     * "separator" as separator.
     */
    @JSON(include = false)
    public String getFullName (String separator,
                               int fromDepth) {
        Node p = getParent();


        if (p == null || getDepth() <= fromDepth)
            return name;

        return p.getFullName(separator, fromDepth) + separator + name;
    }

    /**
     * @brief Gets this node's parent node.
     * @return The parent node or null if the node is a root node.
     */
    @JSON(include = false)
    public Node getParent () {
        return parent;
    }

    @JSON(include = false)
    public int getDepth () {
        return depth;
    }

    @JSON(include = false)
    public List<Node> getChildren () {
        return Collections.unmodifiableList(children);
    }

    /**
     * @brief Retreives children nodes by name.
     * @return The child node or null if no such node exists.
     */
    @JSON(include = false)
    public Node getChild (String name) {
        return childrenByName.get(name);
    }

    @JSON(include = false)
    public Tree getTree () {
        return tree;
    }

    @Override
    public String toString () {
        String ret = "[";


        ret += "name: \"" + getFullName() + "\", ";
        ret += "id: " + getId() + ", ";
        ret += "parentId: " + getParentId() + ", ";
        ret += "children.size(): " + children.size() + "]";
        return ret;
    }

    @JSON(include = false)
    void addChild (Node node) {
        children.add(node);
        childrenByName.put(node.getName(), node);
    }
}