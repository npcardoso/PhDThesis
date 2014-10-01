package io.crowbar.instrumentation.runtime;

import io.crowbar.diagnostic.spectrum.EditableTree;
import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;
import io.crowbar.instrumentation.events.EventListener;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;


import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


public final class Collector {
    private static final Logger logger = LogManager.getLogger(Collector.class);
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

    public Node registerNode (String name,
                              Node.Type type,
                              int parentId) {
        Node n = tree.addNode(name, type, parentId);


        try {
            listener.registerNode(n.getId(), parentId, name, type);
        }
        catch (Throwable e) {
            logger.error(e, e);
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
            listener.registerProbe(p.getId(), p.getNodeId(), p.getType());
        }
        catch (Throwable e) {
            logger.error(e, e);
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
            logger.error(e, e);
        }
    }

    public void transactionEnd (int probeId) {
        try {
            listener.endTransaction(probeId, hitVector.get());
        }
        catch (Throwable e) {
            logger.error(e, e);
        }

        hitVector.reset();
    }

    public void logException (String exceptionClass,
                              String exceptionMessage) {
        try {
            listener.logException(exceptionClass, exceptionMessage);
        }
        catch (Throwable e) {
            logger.error(e, e);
        }
    }

    public void oracle (int probeId,
                        double error,
                        double confidence) {
        try {
            listener.oracle(probeId, error, confidence);
        }
        catch (Throwable e) {
            logger.error(e, e);
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
            logger.error(e, e);
        }
    }

    public Node getRootNode () {
        return tree.getRoot();
    }
}