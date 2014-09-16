package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

import java.io.PrintStream;

public class VerboseListener implements EventListener {
    private final PrintStream out;
    private String prefix = "";
    private String suffix = "";

    private boolean registerNode = true;
    private boolean registerProbe = true;
    private boolean startTransaction = true;
    private boolean endTransaction = true;
    private boolean oracle = true;

    public VerboseListener () {
        this(System.out);
    }

    public VerboseListener (PrintStream out) {
        this.out = out;
    }

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
    public void registerNode (int nodeId,
                              int parentId,
                              String name,
                              Node.Type type) throws Exception {
        if (!registerNode) return;

        String ret = prefix;


        ret += "Registering Node " + nodeId + ": (n:" + name + ", p:" + parentId + ")";
        ret += suffix;
        out.println(ret);
    }

    @Override

    public void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) throws Exception {
        if (!registerProbe) return;

        String ret = prefix;
        ret += "Registering Probe " + probeId + " @ " + nodeId + "(" + type + ")";
        ret += suffix;
        out.println(ret);
    }

    @Override
    public final void startTransaction (int probeId) throws Exception {
        if (!startTransaction) return;

        String ret = prefix;


        ret += "Transaction Start: [probeId: " + probeId + "]";
        ret += suffix;
        out.println(ret);
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
        out.println(ret);
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
        out.println(ret);
    }
}