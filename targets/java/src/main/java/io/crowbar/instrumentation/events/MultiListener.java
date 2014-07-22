package io.crowbar.instrumentation.events;

import java.util.ArrayList;
import java.util.List;


public class MultiListener implements EventListener {
    public void add (EventListener l) {
        assert l != null;
        this.listeners.add(l);
    }

    @Override
    public void startTransaction (int probe_id) {
        for (EventListener cl : listeners) {
            try {
                cl.startTransaction(probe_id);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {
        for (EventListener cl : listeners) {
            try {
                cl.endTransaction(probe_id,
                                  hit_vector);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) {
        for (EventListener cl : listeners) {
            try {
                cl.oracle(probe_id, error, confidence);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private List<EventListener> listeners = new ArrayList<EventListener> ();
}