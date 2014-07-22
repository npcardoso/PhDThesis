package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.Node;


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

    public void registerNode (Node n) {
        try {
            listener.registerNode(n);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) {
        try {
            listener.registerProbe(probe_id, node_id, type);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void transactionStart (int probe_id) {
        if (reset_on_transaction_start)
            hit_vector.reset();

        try {
            listener.startTransaction(probe_id);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void transactionEnd (int probe_id) {
        try {
            listener.endTransaction(probe_id, hit_vector.get());
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        hit_vector.reset();
    }

    public void oracle (int probe_id,
                        double error,
                        double confidence) {
        try {
            listener.oracle(probe_id, error, confidence);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean[] getHitVector (String group_name) {
        return hit_vector.get(group_name);
    }

    public HitVector getHitVector () {
        return hit_vector;
    }

    public Tree getTree () {
        return tree;
    }

    private static Collector collector = new Collector ();

    private HitVector hit_vector = new HitVector() {
        @Override
        protected void registerProbeHook (int probe_id,
                                          ProbeGroup.Probe probe) {
            Collector.this.registerProbe(probe_id,
                                         probe.getNodeId(),
                                         probe.getType());
        }
    };

    private Tree tree = new Tree() {
        @Override
        protected void addChildHook (Node node) {
            registerNode(node);
        }
    };

    private EventListener listener = null;
    public boolean reset_on_transaction_start = true;
}