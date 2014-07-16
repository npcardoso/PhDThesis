package io.crowbar.instrumentation.runtime;
import java.util.*;

public class ProbeSet {
    public int register(ProbeType type) {
        items.add(new Probe(type));
        return items.size() - 1;
    }

    public int register(ProbeType type,
                        String classname) {
        items.add(new Probe(type, classname));
        return items.size() - 1;
    }

    public int register(ProbeType type,
                        String classname,
                        String methodname) {
        items.add(new Probe(type, classname, methodname));
        return items.size() - 1;
    }

    public int register(ProbeType type,
                        String classname,
                        String methodname,
                        int line) {
        items.add(new Probe(type, classname, methodname, line));
        return items.size() - 1;
    }

    public Probe get(int id) {
        return items.get(id);
    }

    public int size() {
        return items.size();
    }

    private Vector<Probe> items = new Vector<Probe>();
}
