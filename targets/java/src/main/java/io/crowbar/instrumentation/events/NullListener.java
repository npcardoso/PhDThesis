package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class NullListener implements EventListener {
    @Override
    public void registerNode (Node n) {}

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) {}

    @Override
    public void startTransaction (int probe_id) {}

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {}

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) {}
}