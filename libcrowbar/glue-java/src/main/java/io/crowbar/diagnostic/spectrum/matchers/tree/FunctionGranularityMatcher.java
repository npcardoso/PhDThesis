package io.crowbar.diagnostic.spectrum.matchers.tree;

import io.crowbar.diagnostic.spectrum.Node;


public class FunctionGranularityMatcher extends AbstractNodeMatcher {
    @Override
    protected boolean keepIfNoChildren () {
        return true;
    }

    @Override
    protected boolean matchNode (Node n) {
        return n.getType() != Node.Type.LINE;
    }
}