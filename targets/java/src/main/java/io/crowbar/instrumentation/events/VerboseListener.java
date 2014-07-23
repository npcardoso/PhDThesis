package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class VerboseListener implements EventListener {
    public String prefix = "";
    public String suffix = "";

    @Override
    public void registerNode (Node n) throws Exception {
        String ret = prefix;


        ret += "Registering Node: " + n;
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) throws Exception {
        String ret = prefix;


        ret += "Registering Probe: [probe_id: " + probe_id;
        ret += ", node_id: " + node_id;
        ret += ", type: " + type + "]";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public void startTransaction (int probe_id) throws Exception {
        String ret = prefix;


        ret += "Transaction Start: [probe_id: " + probe_id + "]";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) throws Exception {
        String ret = prefix;


        ret += "Transaction End: [probe_id: " + probe_id;
        ret += ", hit_vector: [ ";

        for (boolean b : hit_vector)
            ret += b ? "1 " : "0 ";

        ret += "]]" + suffix;
        System.out.println(ret);
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) throws Exception {
        String ret = prefix;


        ret += "Oracle: [probe_id: " + probe_id + ", ";
        ret += "error: " + error + ", ";
        ret += "confidence: " + confidence + "]";
        ret += suffix;
        System.out.println(ret);
    }
}