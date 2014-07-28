package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class NullListener implements EventListener {
    @Override
    public final void registerNode (Node n) throws Exception {}

    @Override
    public final void registerProbe (int probeId,
                                     int nodeId,
                                     ProbeType type) throws Exception {}

    @Override
    public final void startTransaction (int probeId) throws Exception {}

    @Override
    public final void endTransaction (int probeId,
                                      boolean[] hitVector) throws Exception {}

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) throws Exception {}
}