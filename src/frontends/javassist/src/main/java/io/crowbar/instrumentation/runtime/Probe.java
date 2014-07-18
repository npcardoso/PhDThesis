package io.crowbar.instrumentation.runtime;
import java.util.*;

public class Probe extends HashMap<String, String> implements java.io.Serializable {
    Probe (int probe_id,
           ProbeSet parent,
           ProbeType type) {
        this.parent = parent;
        this.probe_id = probe_id;
        put("type", type.name);
    }

    Probe (int probe_id,
           ProbeSet parent,
           ProbeType type,
           String methodname) {
        this(probe_id, parent, type);
        put("method_name", "" + methodname);
    }

    Probe (int probe_id,
           ProbeSet parent,
           ProbeType type,
           String methodname,
           int line) {
        this(probe_id, parent, type, methodname);
        put("line", "" + line);
    }

    public ProbeSet getProbeSet () {
        return parent;
    }

    public int getId () {
        return probe_id;
    }

    private int probe_id;
    private ProbeSet parent;
}