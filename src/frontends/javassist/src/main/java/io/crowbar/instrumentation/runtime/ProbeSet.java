package io.crowbar.instrumentation.runtime;
import java.util.*;

public class ProbeSet {
    public class AlreadyPreparedException extends Exception {}

    public class NotPreparedException extends Exception {}

    public ProbeSet (String name) {
        this.name = name;
    }

    public int register (ProbeType type) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        items.add(new Probe(type));
        return items.size() - 1;
    }

    public int register (ProbeType type,
                         String method_name) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        items.add(new Probe(type, method_name));
        return items.size() - 1;
    }

    public int register (ProbeType type,
                         String method_name,
                         int line) throws AlreadyPreparedException {
        if (isPrepared())
            throw new AlreadyPreparedException();

        items.add(new Probe(type, method_name, line));
        return items.size() - 1;
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

    public int getId () {
        if (!isPrepared())
            throw new NotPreparedException();

        return probeset_id;
    }

    public string getName () {
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