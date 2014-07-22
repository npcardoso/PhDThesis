package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public interface EventListener {
    public void registerNode (Node n);

    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type);

    public void startTransaction (int probe_id);

    public void endTransaction (int probe_id,
                                boolean[] hit_vector);

    public void oracle (int probe_id,
                        double error,
                        double confidence);
}