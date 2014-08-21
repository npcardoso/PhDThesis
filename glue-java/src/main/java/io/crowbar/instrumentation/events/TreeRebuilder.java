package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.AbstractTree;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;

public class TreeRebuilder extends AbstractEventListener {
    /*!
     * This class is needed to have direct access to protected method addNode(Node n).
     */
    private static class TreeBuilder extends AbstractTree {
        public void registerNode (Node n) throws Tree.RegistrationException {
            super.addNode(n);
        }
    }

    private TreeBuilder tree = new TreeBuilder();

    public final Tree getTree () {
        return tree;
    }

    @Override
    public final void registerNode (Node node) throws Exception {
        tree.registerNode(node);
    }
}
