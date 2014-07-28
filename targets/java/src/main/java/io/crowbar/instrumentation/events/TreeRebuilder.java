package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;


public class TreeRebuilder extends Tree implements EventListener {
    public static class DisabledRegistrationException extends RegistrationException {
        public DisabledRegistrationException () {
            super("registerChild method should not be used with a TreeRebuilder");
        }
    }


    @Override
    public final void registerChild (Node node) throws RegistrationException {
        throw new DisabledRegistrationException();
    }

    @Override
    public final void registerNode (Node node) throws Exception {
        _registerChild(node);

        nodes.ensureCapacity(node.getId() + 1);

        while (nodes.size() <= node.getId()) {
            nodes.add(null);
        }

        nodes.set(node.getId(), node);
    }

    @Override
    public final void registerProbe (int probeId,
                                     int nodeId,
                                     ProbeType type) throws Exception {}

    @Override
    public final void startTransaction (int probeId) {}

    @Override
    public final void endTransaction (int probeId,
                                      boolean[] hitVector) {}

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {}
}