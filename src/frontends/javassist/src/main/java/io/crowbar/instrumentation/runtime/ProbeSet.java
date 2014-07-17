package io.crowbar.instrumentation.runtime;
import java.util.*;

public class ProbeSet {
    public ProbeSet(String class_name){
        this.class_name = class_name;
    }

    public int register(ProbeType type) {
        items.add(new Probe(type));
        return items.size() - 1;
    }

    public int register(ProbeType type,
                        String method_name) {
        items.add(new Probe(type, method_name));
        return items.size() - 1;
    }

    public int register(ProbeType type,
                        String method_name,
                        int line) {
        items.add(new Probe(type, method_name, line));
        return items.size() - 1;
    }

    public int size() {
        return items.size();
    }

    public boolean isPrepared () {
        return hitvector != null;
    }

    void prepare() throws Exception {
        if(isPrepared())
            throw new Exception("ProbeSet already prepared");
        hitvector = new boolean[size()];
    }

    public Probe get(int id) {
        return items.get(id);
    }

    public String getClassName() {
        return class_name;
    }

    public boolean[] getHitVector() throws Exception {
        if(!isPrepared())
            throw new Exception("ProbeSet not prepared");
        return hitvector;
    }

    public void resetHitVector() throws Exception {
        if(!isPrepared())
            throw new Exception("ProbeSet not prepared");

        for(int i = 0; i < hitvector.length; i++)
            hitvector[i] = false;
    }

    private String class_name;
    private boolean[] hitvector = null;
    private Vector<Probe> items = new Vector<Probe>();
}
