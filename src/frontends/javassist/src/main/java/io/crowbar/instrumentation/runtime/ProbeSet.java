package io.crowbar.instrumentation.runtime;
import java.util.*;

public class ProbeSet implements java.io.Serializable {
    public static class AlreadyPreparedException extends Exception {}

    public static class NotPreparedException extends Exception {}

    protected ProbeSet () {}

    public ProbeSet (String name) {
        this.name = name;
    }

    public int register (ProbeType type) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        Probe p = new Probe(items.size(), this, type);
        items.add(p);
        return p.getId();
    }

    public int register (ProbeType type,
                         String method_name) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        Probe p = new Probe(items.size(), this, type, method_name);
        items.add(p);
        return p.getId();
    }

    public int register (ProbeType type,
                         String method_name,
                         int line) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        Probe p = new Probe(items.size(), this, type, method_name, line);
        items.add(p);
        return p.getId();
    }

    public int size () {
        return items.size();
    }

    public boolean isPrepared () {
        return probe_set_id >= 0;
    }

    void prepare (int probe_set_id) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        this.probe_set_id = probe_set_id;
    }

    public Probe get (int id) {
        return items.get(id);
    }

    public int getId () throws NotPreparedException {
        if (!isPrepared())
            throw new NotPreparedException();

        return probe_set_id;
    }

    public String getName () {
        return name;
    }

    private int probe_set_id = -1;
    private String name;
    private Vector<Probe> items = new Vector<Probe> ();
}