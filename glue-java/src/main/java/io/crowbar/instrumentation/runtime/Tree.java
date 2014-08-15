package io.crowbar.instrumentation.runtime;

import java.util.Iterator;
import java.util.List;

public interface Tree extends Iterable<Node> {
    public static class RegistrationException extends Exception {
        public RegistrationException (String str) {
            super(str);
        }
    }

    public static class AlreadyBoundException extends RegistrationException {
        public AlreadyBoundException (Node node) {
            super("Node " + node + " is already bound.");
        }
    }

    public static class AlreadyRegisteredException extends RegistrationException {
        public AlreadyRegisteredException (Node node, Node existentNode) {
            super("Node with node_id: " + node.getId() + " already exists. " +
                  "New: " + node + ", " +
                  "Existent: " + existentNode);
        }
    }

    public static class InvalidRootNodeException extends RegistrationException {
        public InvalidRootNodeException (Node node) {
            super("Trying to register root node " + node +
                  " in a tree that already contains a node.");
        }
    }

    public static class NoSuchNodeException extends RegistrationException {
        public NoSuchNodeException (int id) {
            super("Node with node_id: " + id + " does not exist!");
        }
    }


    int size ();
    Node getRoot ();
    Node getNode (int id);
    List<Node> getNodes ();
    Iterator<Node> iterator ();
}