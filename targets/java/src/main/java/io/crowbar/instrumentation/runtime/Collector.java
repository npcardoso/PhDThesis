package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.Tree.Node;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import io.crowbar.instrumentation.events.EventListener;

public class Collector {
    private static Collector collector = new Collector ();
    private EventListener listener = null;
    private final HitVector hitVector = new HitVector();
    private Tree tree = null;
    private boolean resetOnTransactionStart = true;


    public static Collector getDefault () {
        return collector;
    }

    public final void start (String name,
                             EventListener listener) {
        this.listener = listener;
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
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final Probe registerProbe (String groupName,
                                      int nodeId,
                                      ProbeType type) throws RegistrationException {
        Probe p = hitVector.registerProbe(groupName,
                                          nodeId,
                                          type);


        System.err.println("Register: " + p);
        try {
            listener.registerProbe(p.getGlobalId(), nodeId, type);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return p;
    }

    public final void transactionStart (int probeId) {
        if (resetOnTransactionStart)
            hitVector.reset();

        try {
            listener.startTransaction(probeId);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final void transactionEnd (int probeId) {
        try {
            listener.endTransaction(probeId, hitVector.get());
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        hitVector.reset();
    }

    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        try {
            listener.oracle(probeId, error, confidence);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public final boolean[] getHitVector (String className) {
        return hitVector.get(className);
    }

    public final void hit (int globalProbeId) {
        hitVector.hit(globalProbeId);
    }

    public final Tree getTree () {
        return tree;
    }
}