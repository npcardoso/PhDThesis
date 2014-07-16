package io.crowbar.instrumentation.runtime;

import java.util.*;


public class Collector {
    public enum ProbeType {
        TRANSACTION_START("TransactionStart", "transactionStart"),
        TRANSACTION_END("TransactionEnd", "transactionEnd"),
        HIT_PROBE("HitProbe", "hitprobe"),
        ORACLE("Oracle", "oracle");

        public final String name;
        public final String method_name;
        ProbeType(String name,
                  String method_name) {
            this.name = name;
            this.method_name = method_name;
        }
    }

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


    public static Collector getDefault() {
        return collector;
    }


    public void hitprobe (int id) {
        System.out.println("!!!!!!!!! collecting probe @ " + id + "!!!!!!!!!");
    }
// TODO: Probe with state

    public void transactionStart (int id) {
        System.out.println("!!!!!!!!! starting transaction @ " + id + "!!!!!!!!!");
    }

    public void transactionEnd (int id) {
        System.out.println("!!!!!!!!! transaction end @ " + id + "!!!!!!!!!");
    }


    public void oracle (int id,
                        double error,
                        double confidence) {
        System.out.println("!!!!!!!!! collecting oracle (" + error +
                           "," + confidence +
                           ") @ " + id +
                           "!!!!!!!!!");
    }

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

    public Probe getProbe(int id) {
        return items.get(id - 1);
    }

    private static Collector collector = new Collector();

    Vector<Probe> items = new Vector<Probe>();
}
