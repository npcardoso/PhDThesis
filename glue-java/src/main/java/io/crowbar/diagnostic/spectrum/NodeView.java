package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.List;

import flexjson.JSON;

public final class NodeView extends Node {
    private final Node n;
    private final TreeView t;
    private final int id;

    NodeView (TreeView tree,
              Node node,
              int id) {
        this.n = node;
        this.t = tree;
        this.id = id;
    }

    @JSON(include = false)
    public boolean isRoot () {
        return n.isRoot();
    }

    @JSON(include = false)
    public int getId () {
        return id;
    }

    @JSON(include = false)
    public int getPermanentId () {
        return n.getPermanentId();
    }

    @JSON(include = false)
    public String getName () {
        return n.getName();
    }

    @JSON(include = false)
    public Type getType () {
        return n.getType();
    }

    @JSON(name = "n")
    public String getNameWithSymbol () {
        return getType().getSymbol() + getName();
    }

    @JSON(name = "p")
    public int getParentId () {
        return t.getViewIndex(n.getParentId());
    }

    @JSON(include = false)
    public String getFullName () {
        return n.getFullName();
    }

    @JSON(include = false)
    public String getFullName (String separator) {
        return n.getFullName(separator);
    }

    @JSON(include = false)
    public String getFullName (String separator,
                               int fromDepth) {
        return n.getFullName(separator, fromDepth);
    }

    @JSON(include = false)
    public Node getParent () {
        return t.getNode(getParentId());
    }

    @JSON(include = false)
    public int getDepth () {
        return n.getDepth();
    }

    @JSON(include = false)
    public List<Node> getChildren () {
        List<Node> children = new ArrayList<Node> ();

        for (Node c : n.getChildren()) {
            int index = t.getViewIndex(c.getId());

            if (index != -1) {
                children.add(new NodeView(t, c, index));
            }
        }

        return children;
    }

    @JSON(include = false)
    public Node getChild (String name) {
        Node c = n.getChild(name);


        if (c != null) {
            int index = t.getViewIndex(c.getId());

            if (index != -1) {
                return new NodeView(t, c, index);
            }
        }

        return null;
    }

    @JSON(include = false)
    public Tree getTree () {
        return t;
    }

    @Override
    public String toString () {
        String ret = "[";


        ret += "name: \"" + getFullName() + "\", ";
        ret += "id: " + getId() + ", ";
        ret += "parentId: " + getParentId() + ", ";
        ret += "children.size(): " + n.getChildren().size() + "]";
        return ret;
    }

    @JSON(include = false)
    void addChild (Node node) {}
}