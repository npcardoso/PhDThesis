package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

import java.util.ArrayList;
import java.util.List;


public class MultiListener implements EventListener {
    public void add (EventListener l) {
        assert l != null;
        this.listeners.add(l);
    }

    @Override
    public void registerNode (Node n) {
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
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) {
        for (EventListener el : listeners) {
            try {
                el.registerProbe(probe_id, node_id, type);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void startTransaction (int probe_id) {
        for (EventListener el : listeners) {
            try {
                el.startTransaction(probe_id);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {
        for (EventListener el : listeners) {
            try {
                el.endTransaction(probe_id,
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
        for (EventListener el : listeners) {
            try {
                el.oracle(probe_id, error, confidence);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private List<EventListener> listeners = new ArrayList<EventListener> ();
}