package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.ProbeSet;

import java.util.ArrayList;
import java.util.List;


public class MultiListener implements EventListener {
    public void add (EventListener l) {
        assert l != null;
        this.listeners.add(l);
    }

    public void register (ProbeSet ps) {
        for (EventListener cl : listeners) {
            try {
                cl.register(ps);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void startTransaction (Probe p) {
        for (EventListener cl : listeners) {
            try {
                cl.startTransaction(p);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void endTransaction (Probe p,
                                boolean[] hit_vector) {
        for (EventListener cl : listeners) {
            try {
                cl.endTransaction(p, hit_vector);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void oracle (Probe p,
                        double error,
                        double confidence) {
        for (EventListener cl : listeners) {
            try {
                cl.oracle(p, error, confidence);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private List<EventListener> listeners = new ArrayList<EventListener> ();
}