package io.crowbar.instrumentation.runtime;

import java.util.*;


public class Collector {
    public enum ProbeType {
        TRANSACTION_START("TransctionStart"),
        TRANSACTION_END("TransctionStart"),
        HIT_PROBE("HitProbe"),
        ORACLE("Oracle");

        private final String name;
        ProbeType(String name) {
            this.name = name;
        }
    }

    private static Collector collector = new Collector();

    private class Probe extends HashMap<String,String> {
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

    Vector<Probe> items = new Vector<Probe>();

    public static Collector getDefault() {
        return collector;
    }


    public void hitprobe (int id) {
        System.out.println("collecting probe: " + id);
    }
// TODO: Probe with state

    public void transaction (int c_id,
                             boolean start) {}
    public void oracle (int c_id,
                        double error,
                        double confidence) {}

    public int register(ProbeType type) {
        items.add(new Probe(type));
        return items.size();
    }

    public int register(ProbeType type,
                         String classname) {
        items.add(new Probe(type, classname));
        return items.size();
    }

    public int register(ProbeType type,
                         String classname,
                         String methodname) {
        items.add(new Probe(type, classname, methodname));
        return items.size();
    }

    public int register(ProbeType type,
                         String classname,
                         String methodname,
                         int line) {
        items.add(new Probe(type, classname, methodname, line));
        return items.size();
    }



    public void metadata (int c_id,
                          String key,
                          String val) {
        Probe d = items.get(c_id);
        d.put(key, val);
    }

    public
}
