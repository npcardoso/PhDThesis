package io.crowbar.instrumentation.runtime;

import io.crowbar.sandbox.Tree;


import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.events.NullListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;


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

    public void transactionStart (int probe_id) throws Exception {
        /*        ProbeSet ps = probe_store.get(class_name);
         *
         *
         *      assert ps != null;
         *
         *      Probe p = ps.get(probe_id);
         *      assert p != null;
         *
         *      if (reset_on_transaction_start)
         *          hit_vector.reset();
         *
         *      try {
         *          listener.startTransaction(p);
         *      }
         *      catch (Exception e) {
         *          e.printStackTrace();
         *      }
         */}

    public void transactionEnd (int probe_id) throws ProbeSet.NotPreparedException {
        /*ProbeSet ps = probe_store.get(class_name);
         *
         *
         * assert ps != null;
         *
         * Probe p = ps.get(probe_id);
         * assert p != null;
         *
         * try {
         *  listener.endTransaction(p, hit_vector.get());
         * }
         * catch (Exception e) {
         *  e.printStackTrace();
         * }
         *
         * hit_vector.reset();*/
    }

    public void oracle (int probe_id,
                        double error,
                        double confidence) throws ProbeSet.NotPreparedException {
        /*ProbeSet ps = probe_store.get(class_name);
         *
         *
         * assert ps != null;
         *
         * Probe p = ps.get(probe_id);
         * assert p != null;
         *
         * try {
         *  listener.oracle(p, error, confidence);
         * }
         * catch (Exception e) {
         *  e.printStackTrace();
         *  }*/
    }

    public void register (ProbeSet ps) throws Exception {
        ps.prepare(probe_store.getNumProbesSets());
        probe_store.register(ps);

        try {
            listener.register(ps);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public HitVector getHitVector () {
        return hit_vector;
    }

    public boolean[] getHitVector (String group_name) {
        return null; // TODO
    }

    public Tree getTree () {
        return tree;
    }

    private static Collector collector = new Collector ();

    private ProbeStore probe_store = new ProbeStore();
    private HitVector hit_vector = new HitVector();

    private Tree tree = new Tree();

    private EventListener listener = null;
    public boolean reset_on_transaction_start = true;
}