package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public interface EventListener {
    public void registerNode (Node n) throws Exception;

    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) throws Exception;

    public void startTransaction (int probe_id) throws Exception;

    public void endTransaction (int probe_id,
                                boolean[] hit_vector) throws Exception;

    public void oracle (int probe_id,
                        double error,
                        double confidence) throws Exception;
}