package io.crowbar.sandbox;


import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.TreeJSONSerializer;


public class TreeSerialization {
    public static void main (String[] args) {
        Tree t = new Tree("");


        try {
            Node n = t.getRoot();

            Node c1 = n.addChild("foo1");
            Node c2 = n.addChild("foo2");
            Node c11 = c1.addChild("foo1");
            Node c12 = c1.addChild("foo2");
            Node c21 = c2.addChild("foo1");
            Node c22 = c2.addChild("foo2");
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println(TreeJSONSerializer.serialize(t));
    }
}