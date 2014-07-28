package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class VerboseListener implements EventListener {
    public String prefix = "";
    public String suffix = "";

    public boolean enableRegisterNode = true;
    public boolean enableRegisterProbe = true;
    public boolean enableStartTransaction = true;
    public boolean enableEndTransaction = true;
    public boolean enableOracle = true;

    @Override
    public void registerNode (Node n) throws Exception {
        if (!enableRegisterNode) return;

        String ret = prefix;


        ret += "Registering Node: " + n;
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) throws Exception {
        if (!enableRegisterProbe) return;

        String ret = prefix;


        ret += "Registering Probe: [probe_id: " + probe_id;
        ret += ", node_id: " + node_id;
        ret += ", type: " + type + "]";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public void startTransaction (int probe_id) throws Exception {
        if (!enableStartTransaction) return;

        String ret = prefix;


        ret += "Transaction Start: [probe_id: " + probe_id + "]";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) throws Exception {
        if (!enableEndTransaction) return;

        String ret = prefix;


        ret += "Transaction End: [probe_id: " + probe_id;
        ret += ", hit_vector: [ ";

        for (int i = 0; i < hit_vector.length; i++) {
            ret += hit_vector[i] ? (i + " ") : "";
        }

        ret += "]]" + suffix;
        System.out.println(ret);
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) throws Exception {
        if (!enableOracle) return;

        String ret = prefix;


        ret += "Oracle: [probe_id: " + probe_id + ", ";
        ret += "error: " + error + ", ";
        ret += "confidence: " + confidence + "]";
        ret += suffix;
        System.out.println(ret);
    }
}