package io.crowbar.sandbox;


import io.crowbar.instrumentation.runtime.WritableTree;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.TreeJSONSerializer;

import java.util.UUID;


public class TreeSerialization {
    public static void main (String[] args) {
        WritableTree t = new WritableTree("");


        try {
            Node n = t.getRoot();
            createTree(t, n, 10, 15, 4);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println(TreeJSONSerializer.serialize(t));
    }

    public static void createTree (WritableTree t,
                                   Node n,
                                   int children_min,
                                   int children_max,
                                   int depth) throws Exception {
        if (depth <= 0)
            return;

        int children = (int) (Math.random() * (children_max - children_min) + children_min);

        for (int i = 0; i < children; i++) {
            Node n_child = t.addNode(UUID.randomUUID().toString().substring(0, 10), n);
            createTree(t, n_child, children_min, children_max, depth - 1);
        }
    }
}