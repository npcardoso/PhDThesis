package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.AbstractTree;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

public class TreeRebuilder extends AbstractTree implements EventListener {
    public static class DisabledRegistrationException extends RegistrationException {
        public DisabledRegistrationException () {
            super("registerChild method should not be used with a TreeRebuilder");
        }
    }

    @Override
    public final void registerNode (Node node) throws Exception {
        addNode(node);
    }

    @Override
    public final void registerProbe (Probe probe) throws Exception {}

    @Override
    public final void startTransaction (int probeId) {}

    @Override
    public final void endTransaction (int probeId,
                                      String exception,
                                      boolean[] hitVector) {}

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {}
}