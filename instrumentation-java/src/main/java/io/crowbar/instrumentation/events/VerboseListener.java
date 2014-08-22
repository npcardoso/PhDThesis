package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.runtime.Probe;

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
    public final void registerNode (String name, int id, int parentId) throws Exception {
        if (!registerNode) return;

        String ret = prefix;


        ret += "Registering Node: (" + name + ", " + id + ", " + parentId + ")";
        ret += suffix;
        System.out.println(ret);
    }

    @Override
    public final void registerProbe (Probe p) throws Exception {
        if (!registerProbe) return;

        String ret = prefix;
        ret += "Registering Probe: " + p;
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
                                      String exceptionClass,
                                      String exceptionMessage,
                                      boolean[] hitVector) throws Exception {
        if (!endTransaction) return;

        String ret = prefix;


        ret += "Transaction End: [probeId: " + probeId;
        ret += ", exceptionClass: " + exceptionClass;
        ret += ", exceptionMessage: " + exceptionMessage;
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
