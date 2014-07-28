package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import io.crowbar.instrumentation.events.EventListener;

public class Collector {
    private static Collector collector = new Collector ();
    private EventListener listener = null;
    private HitVector hitVector = null;
    private Tree tree = null;
    private boolean resetOnTransactionStart = true;


    public static Collector getDefault () {
        return collector;
    }

    public final void start (String name,
                       EventListener listener) {
        this.listener = listener;
        hitVector = new HitVector() {
            @Override
            public Probe registerProbe (String groupName,
                                        int nodeId,
                                        ProbeType type) {
                Probe p = super.registerProbe(groupName,
                                              nodeId,
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

    public final void registerNode (Node n) {
        try {
            listener.registerNode(n);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) {
        try {
            listener.registerProbe(probeId, nodeId, type);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final void transactionStart (int probeId) {
        if (resetOnTransactionStart)
            hitVector.reset();

        try {
            listener.startTransaction(probeId);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final void transactionEnd (int probeId) {
        try {
            listener.endTransaction(probeId, hitVector.get());
        } catch (Exception e) {
            e.printStackTrace();
        }

        hitVector.reset();
    }

    public final void oracle (int probeId,
                        double error,
                        double confidence) {
        try {
            listener.oracle(probeId, error, confidence);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final HitVector getHitVector () {
        return hitVector;
    }

    public final Tree getTree () {
        return tree;
    }
}