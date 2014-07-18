package io.crowbar.instrumentation.runtime;
import java.util.*;

public class ProbeSet implements java.io.Serializable {
    public static class AlreadyPreparedException extends Exception {}

    public static class NotPreparedException extends Exception {}

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
        return hitvector != null;
    }

    void prepare (int probeset_id) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        hitvector = new boolean[size()];
        this.probeset_id = probeset_id;
    }

    public Probe get (int id) {
        return items.get(id);
    }

    public int getId () throws NotPreparedException {
        if (!isPrepared())
            throw new NotPreparedException();

        return probeset_id;
    }

    public String getName () {
        return name;
    }

    public boolean[] getHitVector () throws NotPreparedException {
        if (!isPrepared())
            throw new NotPreparedException();

        return hitvector;
    }

    public void resetHitVector () throws NotPreparedException {
        if (!isPrepared())
            throw new NotPreparedException();

        for (int i = 0; i < hitvector.length; i++)
            hitvector[i] = false;
    }

    private int probeset_id = -1;
    private String name;
    private boolean[] hitvector = null;
    private Vector<Probe> items = new Vector<Probe> ();
}