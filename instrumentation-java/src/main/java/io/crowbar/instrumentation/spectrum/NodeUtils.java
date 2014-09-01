package io.crowbar.instrumentation.spectrum;

import io.crowbar.diagnostic.spectrum.Node;

import java.util.List;

public class NodeUtils {
    public static String getClassName (Node n) {
        return getNodeName(n, 1);
    }

    public static String getMethodName (Node n) {
        return getNodeName(n, 2);
    }

    public static int getLineNumber (Node n) {
        int depth = n.getDepth();


        try {
            if (depth == 2) {
                List<Node> c = n.getChildren();

                if (!c.isEmpty()) {
                    return Integer.parseInt(c.get(0).getName());
                }
            } else if (depth == 3) {
                return Integer.parseInt(n.getName());
            }
        }
        catch (NumberFormatException e) {}

        return -1;
    }

    private static String getNodeName (Node n,
                                       int depth) {
        if (n != null) {
            if (n.getDepth() > depth) {
                return getNodeName(n.getParent(), depth);
            } else if (n.getDepth() == depth) {
                return n.getName();
            }
        }

        return null;
    }
}