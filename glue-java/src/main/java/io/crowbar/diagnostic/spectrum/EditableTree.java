package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

public final class EditableTree implements Tree {
    private ArrayList<Node> nodes = new ArrayList<Node> ();

    public EditableTree (String rootName) {
        Node node = new Node(this, rootName, nodes.size(), null);
        nodes.add(node);
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

    public Node addNode (String name,
                        int parentId) {
        Node parent = getNode(parentId);
        Node child = new Node(this, name, nodes.size(), parent);
        nodes.add(child);
        return child;
    }
}
