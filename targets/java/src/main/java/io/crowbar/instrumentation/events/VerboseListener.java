package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.ProbeType;

public class VerboseListener implements EventListener {
    private String prefix = "";
    private String suffix = "";

    private boolean registerNode = true;
    private boolean registerProbe = true;
    private boolean startTransaction = true;
    private boolean endTransaction = true;
    private boolean oracle = true;

    public final void setPrefix (String str) {
        prefix = str;
    }

    public final void setSuffix (String str) {
        suffix = str;
    }

    public final void enableRegisterNode (boolean enable) {
        registerNode = enable;
    }

    public final void enableRegisterProbe (boolean enable) {
        registerProbe = enable;
    }

    public final void enableStartTransaction (boolean enable) {
        startTransaction = enable;
    }

    public final void enableEndTransaction (boolean enable) {
        endTransaction = enable;
    }

    public final void enableOracle (boolean enable) {
        oracle = enable;
    }

    @Override
    public final void registerNode (Node n) throws Exception {
        if (!registerNode) return;

        String ret = prefix;


        ret += "Registering Node: " + n;
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public final void registerProbe (int probeId,
                                     int nodeId,
                                     ProbeType type) throws Exception {
        if (!registerProbe) return;

        String ret = prefix;


        ret += "Registering Probe: [probeId: " + probeId;
        ret += ", nodeId: " + nodeId;
        ret += ", type: " + type + "]";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public final void startTransaction (int probeId) throws Exception {
        if (!startTransaction) return;

        String ret = prefix;


        ret += "Transaction Start: [probeId: " + probeId + "]";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public final void endTransaction (int probeId,
                                      boolean[] hitVector) throws Exception {
        if (!endTransaction) return;

        String ret = prefix;


        ret += "Transaction End: [probeId: " + probeId;
        ret += ", hitVector: [ ";

        for (int i = 0; i < hitVector.length; i++) {
            ret += hitVector[i] ? (i + " ") : "";
        }

        ret += "]]" + suffix;
        System.out.println(ret);
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) throws Exception {
        if (!oracle) return;

        String ret = prefix;


        ret += "Oracle: [probeId: " + probeId + ", ";
        ret += "error: " + error + ", ";
        ret += "confidence: " + confidence + "]";
        ret += suffix;
        System.out.println(ret);
    }
}