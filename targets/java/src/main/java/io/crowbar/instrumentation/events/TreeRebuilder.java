package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree;

public class TreeRebuilder extends Tree implements EventListener {
    public static class DisabledRegistrationException extends RegistrationException {
        public DisabledRegistrationException () {
            super("registerChild method should not be used with a TreeRebuilder");
        }
    }


    @Override
    public void registerChild (Node node) throws RegistrationException {
        throw new DisabledRegistrationException();
    }

    @Override
    public void registerNode (Node node) throws Exception {
        _registerChild(node);

        nodes.ensureCapacity(node.getId() + 1);

        while (nodes.size() <= node.getId()) {
            nodes.add(null);
        }

        nodes.set(node.getId(), node);
    }

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) throws Exception {}

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