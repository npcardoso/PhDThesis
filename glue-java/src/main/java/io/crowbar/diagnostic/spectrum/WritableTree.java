package io.crowbar.diagnostic.spectrum;

public class WritableTree extends AbstractTree {
    public WritableTree (String name) {
        super(name);
    }

    public final Node addNode (String name,
                               Node parent) throws Tree.RegistrationException {
        Node n = new Node(name,
                          size(),
                          parent.getId());


        addNode(n);
        return n;
    }
}
