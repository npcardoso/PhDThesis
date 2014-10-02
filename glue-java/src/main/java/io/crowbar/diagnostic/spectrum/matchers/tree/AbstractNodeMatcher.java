package io.crowbar.diagnostic.spectrum.matchers.tree;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;

import java.util.BitSet;

public abstract class AbstractNodeMatcher implements TreeMatcher {
    @Override
    public BitSet matchNodes (Tree t) {
        BitSet bs = new BitSet();

        boolean keep = keepIfNoChildren();
        Node n = t.getRoot(); // tree root is not matched


        bs.set(n.getId());

        for (Node c : n.getChildren()) {
            match(bs, c, keep);
        }

        return bs;
    }

    private boolean match (BitSet bs,
                           Node n,
                           boolean keep) {
        if (n == null) return false;

        if (n.getChildren().size() == 0) {
            // leaf node
            if (matchNode(n)) {
                bs.set(n.getId());
                return true;
            } else return false;
        } else {
            if (matchNode(n)) {
                bs.set(n.getId());
                boolean hasMatched = false;

                for (Node c : n.getChildren()) {
                    hasMatched |= match(bs, c, keep);
                }

                if (!hasMatched && !keep) {
                    bs.clear(n.getId());
                    return false;
                } else {
                    return true;
                }
            } else return false;
        }
    }

    protected abstract boolean keepIfNoChildren ();
    protected abstract boolean matchNode (Node n);
}