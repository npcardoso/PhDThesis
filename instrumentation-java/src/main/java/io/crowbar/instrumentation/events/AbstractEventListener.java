package io.crowbar.instrumentation.events;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.ProbeType;

public abstract class AbstractEventListener implements EventListener {
    @Override
    public void registerNode (int nodeId,
                              int parentId,
                              String name,
                              Node.Type type) throws Exception {}

    @Override
    public void registerProbe (int probeId,
                               int nodeId,
                               ProbeType type) throws Exception {}

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