package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

public abstract class AbstractEventListener implements EventListener {
    @Override
    public void registerNode (Node node) throws Exception {}

    @Override
    public void registerProbe (Probe probe) throws Exception {}

    @Override
    public void startTransaction (int probeId) throws Exception {}

    @Override
    public void endTransaction (int probeId,
                                String exceptionClass,
                                String exceptionMessage,
                                boolean[] hitVector) throws Exception {}

    @Override
    public void oracle (int probeId,
                        double error,
                        double confidence) throws Exception {}
}