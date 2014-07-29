package io.crowbar.sandbox;


import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.TreeJSONSerializer;


public class TreeSerialization {
    public static void main (String[] args) {
        Tree t = new Tree("");


        try {
            Node n = t.getRoot();

            Node c1 = t.addNode("foo1", n);
            Node c2 = t.addNode("foo2", n);
            Node c11 = t.addNode("foo1", c1);
            Node c12 = t.addNode("foo2", c1);
            Node c21 = t.addNode("foo1", c2);
            Node c22 = t.addNode("foo2", c2);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println(TreeJSONSerializer.serialize(t));
    }
}