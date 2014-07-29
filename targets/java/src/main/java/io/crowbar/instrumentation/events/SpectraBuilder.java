package io.crowbar.instrumentation.events;

import io.crowbar.diagnosis.Spectra;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

public class SpectraBuilder extends Spectra implements EventListener {
    private int transactionId = 0;

    @Override
    public final void registerNode (Node node) throws Exception {}

    @Override
    public final void registerProbe (Probe probe) throws Exception {}

    @Override
    public final void startTransaction (int probeId) {}

    @Override
    public final void endTransaction (int probeId,
                                      boolean[] hitVector) {
        setActivity(transactionId++, hitVector);
    }

    @Override
    public final void oracle (int probeId,
                              double error,
                              double confidence) {
        setError(transactionId, error > 0);
    }
}