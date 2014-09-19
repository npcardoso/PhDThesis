package io.crowbar.diagnostic.spectrum;

import io.crowbar.util.ViewUtils;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.Iterator;
import java.util.List;

public final class TreeView extends AbstractTree {
    private final Tree t;
    private int[] nodes;

    public TreeView (Tree t,
                     BitSet nodes) {
        this.t = t;
        this.nodes = ViewUtils.toMappingArray(nodes);
    }

    public int getNodeMapping (int id) {
        return nodes[id];
    }

    @Override
    public int size () {
        return nodes.length;
    }

    @Override
    public Node getRoot () {
        Node n = t.getRoot();


        return new NodeView(this, n, n.getId());
    }

    @Override
    public Node getNode (int id) {
        Node n = t.getNode(getNodeMapping(id));


        return new NodeView(this, n, id);
    }

    @Override
    public List<Node> getNodes () {
        List<Node> list = new ArrayList<Node> ();

        for (int i = 0; i < size(); i++) {
            list.add(getNode(i));
        }

        return list;
    }

    @Override
    public Iterator<Node> iterator () {
        return getNodes().iterator();
    }

    public int getViewIndex (int id) {
        for (int i = 0; i < size(); i++) {
            if (nodes[i] == id)
                return i;
        }

        return -1;
    }

    public List<Double> updateScores (List<Double> scores) {
        return removeScores(updateParents(scores));
    }

    private List<Double> updateParents (List<Double> scores) {
        List<Double> newScores = new ArrayList<Double> ();

        for (int i = 0; i < scores.size(); i++) {
            Node n = t.getNode(i);
            Double nodeScore = scores.get(i);

            if (Double.isNaN(nodeScore) || nodeScore == -1) {
                nodeScore = -1d;

                for (Node c : n.getChildren()) {
                    Double childScore = scores.get(c.getId());

                    if (!Double.isNaN(childScore)) {
                        nodeScore = Math.max(nodeScore, childScore);
                    }
                }
            }

            newScores.add(nodeScore);
        }

        return newScores;
    }

    private List<Double> removeScores (List<Double> scores) {
        List<Double> newScores = new ArrayList<Double> ();

        for (int i : nodes) {
            newScores.add(scores.get(i));
        }

        return newScores;
    }
}