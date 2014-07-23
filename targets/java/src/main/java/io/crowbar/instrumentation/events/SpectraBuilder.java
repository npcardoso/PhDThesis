package io.crowbar.instrumentation.events;

import io.crowbar.diagnosis.Spectra;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.Node;

public class SpectraBuilder extends Spectra implements EventListener {
    private int transactionId = 0;

    @Override
    public void registerNode (Node node) throws Exception {}

    @Override
    public void registerProbe (int probe_id,
                               int node_id,
                               ProbeType type) throws Exception {}

    @Override
    public void startTransaction (int probe_id) {}

    @Override
    public void endTransaction (int probe_id,
                                boolean[] hit_vector) {
        setActivity(transactionId++, hit_vector);
    }

    @Override
    public void oracle (int probe_id,
                        double error,
                        double confidence) {
        setError(transactionId, error > 0);
    }
}