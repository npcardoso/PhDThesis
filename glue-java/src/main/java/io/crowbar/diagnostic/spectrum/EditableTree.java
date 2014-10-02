package io.crowbar.diagnostic.spectrum;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

public final class EditableTree extends AbstractTree {
    private ArrayList<Node> nodes = new ArrayList<Node> ();

    public EditableTree (String rootName) {
        Node node = new Node(this, rootName, Node.Type.PACKAGE, nodes.size(), null);


        nodes.add(node);
    }

    @Override
    public int size () {
        return nodes.size();
    }

    @Override
    public Node getRoot () {
        return getNode(0);
    }

    @Override
    public Node getNode (int id) {
        if (id < 0 || id >= nodes.size())
            return null;

        return nodes.get(id);
    }

    @Override
    public List<Node> getNodes () {
        return Collections.unmodifiableList(nodes);
    }

    @Override
    public Iterator<Node> iterator () {
        return nodes.iterator();
    }

    public Node addNode (String name,
                         Node.Type type,
                         int parentId) {
        Node parent = getNode(parentId);


        assert (parent.getType() == null || type == null || parent.getType().canContain(type));

        Node child = new Node(this, name, type, nodes.size(), parent);


        nodes.add(child);
        return child;
    }
}