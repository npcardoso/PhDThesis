package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.spectrum.matchers.tree.TreeMatcher;

import java.util.BitSet;

public class TreeViewFactory {
    private final Tree tree;
    private final BitSet nodes = new BitSet();

    public TreeViewFactory (Tree tree) {
        this.tree = tree;
        nodes.set(0, tree.size());
    }

    public TreeView getView () {
        return new TreeView(tree, nodes);
    }

    public void addStage (TreeMatcher matcher) {
        TreeView tv = getView();
        BitSet bs = matcher.matchNodes(tv);


        assert (bs != null);
        assert (bs.length() <= tv.size());

        for (int i = 0; i < tv.size(); i++) {
            if (!bs.get(i)) {
                int original = tv.getNodeMapping(i);
                nodes.clear(original);
            }
        }
    }
}