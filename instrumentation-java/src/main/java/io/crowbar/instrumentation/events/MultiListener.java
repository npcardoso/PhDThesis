package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

import java.util.ArrayList;
import java.util.List;


public final class MultiListener
implements EventListener {
    public final void add (EventListener l) {
        assert l != null;
        this.listeners.add(l);
    }

    @Override
    public void registerNode (int nodeId,
                              int parentId,
                              String name,
                              Node.Type type) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.registerNode(nodeId, parentId, name, type);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.registerProbe(probeId, nodeId, type);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void startTransaction (int probeId) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.startTransaction(probeId);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void endTransaction (int probeId,
                                boolean[] hitVector) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.endTransaction(probeId, hitVector);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void logException (String exceptionClass,
                              String exceptionMessage) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.logException(exceptionClass, exceptionMessage);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void oracle (int probeId,
                        double error,
                        double confidence) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.oracle(probeId, error, confidence);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private List<EventListener> listeners = new ArrayList<EventListener> ();
}