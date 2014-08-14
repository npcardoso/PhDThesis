package io.crowbar.instrumentation.runtime;

import java.io.Serializable;

public class Probe implements Serializable {
    private final int id;
    private final int nodeId;
    private final ProbeType type;

    public Probe (int id,
                  int nodeId,
                  ProbeType type) {
        this.id = id;
        this.nodeId = nodeId;
        this.type = type;
    }

    public Probe (Probe p) {
        this.id = p.id;
        this.nodeId = p.nodeId;
        this.type = p.type;
    }

    public final int getId () {
        return id;
    }

    public final int getNodeId () {
        return nodeId;
    }

    public final ProbeType getType () {
        return type;
    }

    @Override
    public String toString () {
        String ret = "[Probe: ";


        ret += "(id: " + id;
        ret += ", nodeId: " + nodeId;
        ret += ", type: " + type + ")]";
        return ret;
    }

    protected Probe () {
        this(-1, -1, null);
    }
}