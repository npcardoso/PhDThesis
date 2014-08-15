package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

import java.util.ArrayList;
import java.util.List;


public class MultiListener implements EventListener {
    public final void add (EventListener l) {
        assert l != null;
        this.listeners.add(l);
    }

    @Override
    public final void registerNode (Node n) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.registerNode(n);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.registerProbe(probe);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public final void startTransaction (int probeId) throws Exception {
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
    public final void endTransaction (int probeId,
                                      String exceptionClass,
                                      String exceptionMessage,
                                      boolean[] hitVector) throws Exception {
        for (EventListener el : listeners) {
            try {
                el.endTransaction(probeId,
                                  exceptionClass,
                                  exceptionMessage,
                                  hitVector);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public final void oracle (int probeId,
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