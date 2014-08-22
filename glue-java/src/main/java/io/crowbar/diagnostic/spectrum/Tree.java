package io.crowbar.diagnostic.spectrum;

import java.util.Iterator;
import java.util.List;

public interface Tree extends Iterable<Node> {
    int size ();
    Node getRoot ();
    Node getNode (int id);
    List<Node> getNodes ();
    Iterator<Node> iterator ();
}
