package io.crowbar.diagnostic.spectrum;

import io.crowbar.diagnostic.spectrum.Node;

import java.util.ArrayList;
import java.util.List;

public class NodeUtils {
    public static String getClassName (Node n) {
        while (n != null && n.getType() != Node.Type.CLASS)
            n = n.getParent();

        if (n == null)
            return null;

        return n.getFullNameWithSymbol(1);
    }

    public static String getMethodName (Node n) {
        try {
            if (n.getType() == Node.Type.LINE)
                n = n.getParent();

            if (n.getType() == Node.Type.METHOD)
                return n.getName();
        }
        catch (NumberFormatException e) {}

        return null;
    }

    public static int getLineNumber (Node n) {
        try {
            if (n.getType() == Node.Type.LINE)
                return Integer.parseInt(n.getName());

            if (n.getType() == Node.Type.METHOD) {
                List<Node> c = n.getChildren();

                if (!c.isEmpty() && c.get(0).getType() == Node.Type.LINE)
                    return Integer.parseInt(c.get(0).getName());
            }
        }
        catch (NumberFormatException e) {}

        return -1;
    }

    // TODO: Implement List as a view instead of actually creating a list
    public static List<Probe> getNodeProbes (Spectrum s,
                                             int nodeId) {
        List<Probe> nodeProbes = new ArrayList<Probe> ();

        for (Probe p : s.getProbes()) {
            if (p.getNodeId() == nodeId)
                nodeProbes.add(p);
        }

        return nodeProbes;
    }
}