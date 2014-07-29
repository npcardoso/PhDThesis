package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

public class NullListener implements EventListener {
    @Override
    public final void registerNode (Node n) throws Exception {}

    @Override
    public final void registerProbe (Probe probe) throws Exception {}

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