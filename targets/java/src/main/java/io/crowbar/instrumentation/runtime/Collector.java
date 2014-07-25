package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
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
    private static Collector collector = new Collector ();
    private EventListener listener = null;
    private HitVector hit_vector = null;
    private Tree tree = null;
    public boolean reset_on_transaction_start = true;


    public static Collector getDefault () {
        return collector;
    }

    public void start (String name,
                       EventListener listener) {
        this.listener = listener;
        hit_vector = new HitVector() {
            @Override
            public Probe registerProbe (String group_name,
                                        int node_id,
                                        ProbeType type) {
                Probe p = super.registerProbe(group_name,
                                              node_id,
                                              type);


                Collector.this.registerProbe(p.getGlobalId(),
                                             p.getNodeId(),
                                             p.getType());
                return p;
            }
        };
        this.tree = new Tree(name) {
            @Override
            protected void registerChild (Node node) throws RegistrationException {
                super.registerChild(node);
                registerNode(node);
            }
        };
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

    public HitVector getHitVector () {
        return hit_vector;
    }

    public Tree getTree () {
        return tree;
    }
}