package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;
import io.crowbar.instrumentation.events.EventListener;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.diagnostic.spectrum.WritableTree;

public final class Collector {
    private static Collector collector = null;
    private final EventListener listener;
    private final HitVector hitVector = new HitVector();
    private final WritableTree tree;
    private boolean resetOnTransactionStart = true;


    public static Collector instance () {
        assert collector != null;
        return collector;
    }

    public static void start (String name,
                              EventListener listener) {
        collector = new Collector(name, listener);
    }

    private Collector (String name,
                       EventListener listener) {
        this.listener = listener;
        tree = new WritableTree(name);
        registerNode(tree.getRoot());
    }

    public void registerNode (Node n) {
        try {
            listener.registerNode(n);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public HitProbe registerProbe (String groupName,
                                   int nodeId,
                                   ProbeType type) {
            HitProbe p = hitVector.registerProbe(groupName,
                                                 nodeId,
                                                 type);
        try {
            listener.registerProbe(p);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
        return p;
    }

    public void transactionStart (int probeId) {
        try {
            if (resetOnTransactionStart)
                hitVector.reset();

            listener.startTransaction(probeId);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public void transactionEnd (int probeId) {
        try {
            listener.endTransaction(probeId, null, null, hitVector.get());
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
        try {
            hitVector.reset();
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public void transactionEnd (int probeId,
                                Throwable exception) {
        try {
            String exceptionClass = null;
            String exceptionMessage = null;

            if (exception != null) {
                exceptionClass = exception.getClass().getName();
                exceptionMessage = exception.getMessage();
            }

            listener.endTransaction(probeId,
                                    exceptionClass,
                                    exceptionMessage,
                                    hitVector.get());
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
        try {
            hitVector.reset();
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public void oracle (int probeId,
                        double error,
                        double confidence) {
        try {
            listener.oracle(probeId, error, confidence);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public boolean[] getHitVector (String className) {
        return hitVector.get(className);
    }

    public void hit (int globalProbeId) {
        try {
            hitVector.hit(globalProbeId);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
    }

    public Node getRootNode () {
        return tree.getRoot();
    }

    public Node addNode (String name,
                         Node parent) throws Tree.RegistrationException {
        Node n = tree.addNode(name, parent);


        registerNode(n);
        return n;
    }
}
