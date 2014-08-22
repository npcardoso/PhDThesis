package io.crowbar.instrumentation.runtime;

import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;
import io.crowbar.instrumentation.events.EventListener;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.diagnostic.spectrum.Tree;
import io.crowbar.diagnostic.spectrum.EditableTree;

public final class Collector {
    private static Collector collector = null;
    private final EventListener listener;
    private final HitVector hitVector = new HitVector();
    private final EditableTree tree;
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
        tree = new EditableTree(name);
    }

    public Node registerNode(String name,
                             int parentId) {
        Node n = tree.addNode(name, parentId);
        try {
            listener.registerNode(name, n.getId(), parentId);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }
        return n;
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
}
