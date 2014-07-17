package io.crowbar.instrumentation.runtime;
import java.util.*;

class Probe extends HashMap<String,String> {
    Probe(ProbeType type) {
        put("type", type.name);
    }


    Probe(ProbeType type,
          String methodname) {
        this(type);
        put("method_name", "" + methodname);
    }

    Probe(ProbeType type,
          String methodname,
          int line) {
        this(type, methodname);
        put("line", "" + line);
    }
}
