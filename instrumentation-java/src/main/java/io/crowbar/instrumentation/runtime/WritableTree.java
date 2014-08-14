package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

public class WritableTree extends AbstractTree {
    public WritableTree (String name) {
        super(name);
    }

    public final Node addNode (String name,
                               Node parent) throws RegistrationException {
        Node n = new Node(name,
                          size(),
                          parent.getId());


        addNode(n);
        return n;
    }
}