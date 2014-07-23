package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class NullListener implements EventListener {
    @Override
    public void registerNode (Node n) throws Exception {}

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) throws Exception {}

    @Override
    public void startTransaction (int probe_id) throws Exception {}

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) throws Exception {}

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) throws Exception {}
}