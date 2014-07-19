package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.NullListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class Collector {
    public static Collector getDefault () {
        return collector;
    }

    public Collector () {
        setListener(new NullListener());
    }

    public void setListener (EventListener listener) {
        this.listener = listener;
    }

    public void transactionStart (String class_name,
                                  int probe_id) throws Exception {
        ProbeSet ps = probe_store.get(class_name);


        assert ps != null;

        Probe p = ps.get(probe_id);
        assert p != null;

        if (reset_on_transaction_start)
            resetHitVectors();

        hit_vectors.get(ps.getId())[probe_id] = true;

        try {
            listener.startTransaction(p);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void transactionEnd (String class_name,
                                int probe_id) throws ProbeSet.NotPreparedException {
        ProbeSet ps = probe_store.get(class_name);


        assert ps != null;

        Probe p = ps.get(probe_id);
        assert p != null;


        hit_vectors.get(ps.getId())[probe_id] = true;

        boolean[] hit_vector = collectHitVectors();

        try {
            listener.endTransaction(p, hit_vector);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        resetHitVectors();
    }

    public void oracle (String class_name,
                        int probe_id,
                        double error,
                        double confidence) throws ProbeSet.NotPreparedException {
        ProbeSet ps = probe_store.get(class_name);


        assert ps != null;

        Probe p = ps.get(probe_id);
        assert p != null;

        hit_vectors.get(ps.getId())[probe_id] = true;

        try {
            listener.oracle(p, error, confidence);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void register (ProbeSet ps) throws Exception {
        ps.prepare(probe_store.getNumProbesSets());
        probe_store.register(ps);

        hit_vectors.add(new boolean[ps.size()]);
        try {
            listener.register(ps);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean[] getHitVector (int probe_set_id) {
        return hit_vectors.get(probe_set_id);
    }

    public boolean[] getHitVector (String probe_set_name) {
        try {
            ProbeSet ps = probe_store.get(probe_set_name);
            return hit_vectors.get(ps.getId());
        }
        catch (ProbeSet.NotPreparedException e) {
            e.printStackTrace();
        }

        return null;
    }

    public boolean[] collectHitVectors () throws ProbeSet.NotPreparedException {
        boolean[] ret = new boolean[probe_store.getNumProbes()];
        int i = 0;

        for (boolean[] hv : hit_vectors)
            for (int j = 0; j < hv.length; j++)
                ret[i++] = hv[j];

        return ret;
    }

    public void resetHitVectors () {
        for (boolean[] hv : hit_vectors)
            for (int j = 0; j < hv.length; j++)
                hv[j] = false;
    }

    private static Collector collector = new Collector ();


    private ProbeStore probe_store = new ProbeStore();
    private List<boolean[]> hit_vectors = new ArrayList<boolean[]> ();

    private EventListener listener;
    public boolean reset_on_transaction_start = true;
}