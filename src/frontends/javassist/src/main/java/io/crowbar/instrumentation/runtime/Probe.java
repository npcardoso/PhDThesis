package io.crowbar.instrumentation.runtime;
import java.util.*;

class Probe extends HashMap<String,String> {
    Probe(ProbeType type) {
        put("type", type.name);
    }

    Probe(ProbeType type,
          String classname) {
        this(type);
        put("class_name", classname);
    }

    Probe(ProbeType type,
          String classname,
          String methodname) {
        this(type, classname);
        put("method_name", "" + methodname);
    }

    Probe(ProbeType type,
          String classname,
          String methodname,
          int line) {
        this(type, classname, methodname);
        put("line", "" + line);
    }
}
