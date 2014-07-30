package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.AbstractTree;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

public class TreeRebuilder extends AbstractEventListener {
    /*!
     * This class is needed to have direct access to protected method addNode(Node n).
     */
    private static class TreeBuilder extends AbstractTree {
        public void registerNode (Node n) throws RegistrationException {
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

    @Override
    public final void registerProbe (Probe probe) throws Exception {}

    @Override
    public final void startTransaction (int probeId) {}

    @Override
    public final void endTransaction (int probeId,
                                      String exception,
                                      boolean[] hitVector) {}

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {}
}